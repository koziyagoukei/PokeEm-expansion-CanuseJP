// JoyBus transport and reliable-frame responder for GC Extension Mode.
//
// The serial IRQ part is a C port of the queue scheme used by LinkCube from
// afska/gba-link-connection (MIT, vendored unmodified in gba-gc-extension):
// RESET clears the queues, RECEIVE pushes JOY_RECV, SEND loads the next
// queued word (or 0 when idle) into JOY_TRANS. pokeemerald-expansion has no
// C++ build rule and no static constructors, so the header itself is not used.
//
// The responder mirrors the validated Phase 1 PoC client: sequences start at
// 1 with HELLO, an identical retransmission replays the cached reply, a
// different request with a reused sequence is NACKed.
#include "global.h"
#include "main.h"
#include "malloc.h"
#include "gpu_regs.h"
#include "gc_companion_link.h"
#include "gc_ext/round_trip.h"

#define QUEUE_SIZE 64          // words; one frame is at most WIRE_WORDS (60)
#define SILENCE_TIMEOUT 120    // frames without any word while connected

#define JOYCNT_RESET   (1 << 0)
#define JOYCNT_RECEIVE (1 << 1)
#define JOYCNT_SEND    (1 << 2)
#define JOYCNT_IRQ     (1 << 6)
#define RCNT_JOYBUS    0xC000
#define REG_JOYCNT_16  (*(vu16 *)REG_ADDR_JOYCNT)
#define REG_JOY_RECV_L_16 (*(vu16 *)REG_ADDR_JOY_RECV_L)
#define REG_JOY_RECV_H_16 (*(vu16 *)REG_ADDR_JOY_RECV_H)
#define REG_JOY_TRANS_L_16 (*(vu16 *)REG_ADDR_JOY_TRANS_L)
#define REG_JOY_TRANS_H_16 (*(vu16 *)REG_ADDR_JOY_TRANS_H)

struct WordQueue
{
    u32 words[QUEUE_SIZE];
    u8 head;
    u8 count;
};

struct GcLink
{
    // Shared with the serial IRQ.
    struct WordQueue incoming;
    struct WordQueue outgoing;
    vbool8 resetSeen;
    vbool8 overflowSeen;
    // Main loop only.
    Parser parser;
    Frame previous;
    Frame cached;
    Frame request;
    Frame reply;
    struct GcLinkStats stats;
    u32 lastActivity;
    bool8 connected;
    // Registers restored on close.
    IntrFunc savedSerialIntr;
    u16 savedRcnt;
    u16 savedIe;
};

static EWRAM_DATA struct GcLink *sLink = NULL;

static bool8 QueuePush(struct WordQueue *queue, u32 word)
{
    if (queue->count >= QUEUE_SIZE)
        return FALSE;
    queue->words[(queue->head + queue->count) % QUEUE_SIZE] = word;
    queue->count++;
    return TRUE;
}

static bool8 QueuePop(struct WordQueue *queue, u32 *word)
{
    if (queue->count == 0)
        return FALSE;
    *word = queue->words[queue->head];
    queue->head = (queue->head + 1) % QUEUE_SIZE;
    queue->count--;
    return TRUE;
}

static void SerialIntr_GcJoyBus(void)
{
    struct GcLink *link = sLink;
    u16 cnt = REG_JOYCNT_16;
    u16 ack = cnt & (JOYCNT_RESET | JOYCNT_RECEIVE | JOYCNT_SEND);

    if (link != NULL)
    {
        if (cnt & JOYCNT_RESET)
        {
            link->incoming.count = 0;
            link->outgoing.count = 0;
            link->resetSeen = TRUE;
        }
        if (cnt & JOYCNT_RECEIVE)
        {
            u32 word = REG_JOY_RECV_L_16 | ((u32)REG_JOY_RECV_H_16 << 16);
            if (!QueuePush(&link->incoming, word))
                link->overflowSeen = TRUE;
        }
        if (cnt & JOYCNT_SEND)
        {
            u32 word = 0;
            QueuePop(&link->outgoing, &word);
            REG_JOY_TRANS_H_16 = word >> 16;
            REG_JOY_TRANS_L_16 = word;
        }
    }
    // Writing 1 acknowledges exactly the events handled above.
    REG_JOYCNT_16 = ack | JOYCNT_IRQ;
    INTR_CHECK |= INTR_FLAG_SERIAL;
    gMain.intrCheck |= INTR_FLAG_SERIAL;
}

bool8 GcLink_Open(void)
{
    if (sLink != NULL)
        return TRUE;
    sLink = AllocZeroedUnchecked(sizeof(*sLink));
    if (sLink == NULL)
        return FALSE;

    REG_IME = 0;
    sLink->savedSerialIntr = gIntrTable[1];
    sLink->savedRcnt = REG_RCNT;
    sLink->savedIe = REG_IE & INTR_FLAG_SERIAL;
    gIntrTable[1] = SerialIntr_GcJoyBus;
    REG_RCNT = RCNT_JOYBUS;
    REG_JOYCNT_16 = JOYCNT_RESET | JOYCNT_RECEIVE | JOYCNT_SEND | JOYCNT_IRQ;
    REG_IF = INTR_FLAG_SERIAL;
    REG_IE |= INTR_FLAG_SERIAL;
    REG_IME = 1;
    sLink->lastActivity = gMain.vblankCounter1;
    return TRUE;
}

void GcLink_Close(void)
{
    if (sLink == NULL)
        return;
    REG_IME = 0;
    REG_JOYCNT_16 = JOYCNT_RESET | JOYCNT_RECEIVE | JOYCNT_SEND; // IRQ off, clear pending
    if (!sLink->savedIe)
        REG_IE &= ~INTR_FLAG_SERIAL;
    REG_IF = INTR_FLAG_SERIAL;
    REG_RCNT = sLink->savedRcnt;
    gIntrTable[1] = sLink->savedSerialIntr;
    REG_IME = 1;
    Free(sLink);
    sLink = NULL;
}

bool8 GcLink_IsConnected(void)
{
    return sLink != NULL && sLink->connected;
}

bool8 GcLink_HasPendingOutput(void)
{
    return sLink != NULL && sLink->outgoing.count != 0;
}

const struct GcLinkStats *GcLink_GetStats(void)
{
    return sLink != NULL ? &sLink->stats : NULL;
}

static void Send(const u32 *words, u32 count)
{
    u32 i;
    u16 ime = REG_IME;

    REG_IME = 0;
    if (count <= QUEUE_SIZE - sLink->outgoing.count)
    {
        for (i = 0; i < count; i++)
            QueuePush(&sLink->outgoing, words[i]);
    }
    else
    {
        sLink->stats.overflows++;
    }
    REG_IME = ime;
}

static void SendFrame(const Frame *frame)
{
    u32 words[WIRE_WORDS];
    u32 count = wire_encode(frame, words);
    if (count != 0)
        Send(words, count);
}

static void SendNack(u32 seq)
{
    Frame *nack = &sLink->reply;
    nack->type = NACK;
    nack->seq = seq;
    nack->len = 0;
    sLink->stats.nacks++;
    SendFrame(nack);
}

static void ResetSession(void)
{
    sLink->parser.count = 0;
    sLink->previous.seq = 0;
    sLink->connected = FALSE;
}

static bool8 HandleRequest(const Frame *request, GcFrameHandler handler)
{
    struct GcLink *link = sLink;
    Frame *reply = &link->reply;

    if (link->previous.seq != 0 && request->seq == link->previous.seq)
    {
        if (request->type != link->previous.type || request->len != link->previous.len
         || memcmp(request->payload, link->previous.payload, request->len) != 0)
        {
            SendNack(request->seq);
            return FALSE;
        }
        link->stats.duplicates++;
        SendFrame(&link->cached);
        return FALSE;
    }
    if (request->seq != link->previous.seq + 1 || (link->previous.seq == 0 && request->type != HELLO))
    {
        SendNack(request->seq);
        return FALSE;
    }
    reply->seq = request->seq;
    reply->type = ACK;
    reply->len = 0;
    if (!handler(request, reply))
    {
        SendNack(request->seq);
        return FALSE;
    }
    link->previous = *request;
    link->cached = *reply;
    link->stats.requests++;
    SendFrame(reply);
    return TRUE;
}

u32 GcLink_Poll(GcFrameHandler handler)
{
    struct GcLink *link = sLink;
    u32 events = 0;
    u32 word;
    u16 ime;
    bool8 gotWord;

    if (link == NULL)
        return 0;

    ime = REG_IME;
    REG_IME = 0;
    if (link->resetSeen)
    {
        link->resetSeen = FALSE;
        link->stats.resets++;
        if (link->connected || link->previous.seq != 0)
            events |= GC_LINK_EVENT_RESET;
        ResetSession();
    }
    if (link->overflowSeen)
    {
        link->overflowSeen = FALSE;
        link->stats.overflows++;
        link->parser.count = 0;
    }
    REG_IME = ime;

    for (;;)
    {
        ime = REG_IME;
        REG_IME = 0;
        gotWord = QueuePop(&link->incoming, &word);
        REG_IME = ime;
        if (!gotWord)
            break;

        link->lastActivity = gMain.vblankCounter1;
        if (link->previous.seq == 0 && link->parser.count == 0 && word == RAW_PROBE)
        {
            static const u32 sRawValues[] = {RAW_VALUES, 0, 0xFFFFFFFF, RAW_PATTERN};
            Send(sRawValues, ARRAY_COUNT(sRawValues));
        }
        else if (link->previous.seq == 0 && link->parser.count == 0 && word == RAW_ACK)
        {
            static const u32 sRawConfirm = RAW_CONFIRM;
            Send(&sRawConfirm, 1);
            link->connected = TRUE;
            events |= GC_LINK_EVENT_CONNECTED;
        }
        else
        {
            switch (wire_feed(&link->parser, word, &link->request))
            {
            case FRAME_OK:
                if (HandleRequest(&link->request, handler))
                    events |= GC_LINK_EVENT_FRAME;
                break;
            case BAD_CRC:
                link->stats.crcErrors++;
                SendNack(0);
                break;
            case BAD_HEADER:
                SendNack(0);
                break;
            default:
                break;
            }
        }
    }

    if (link->connected && gMain.vblankCounter1 - link->lastActivity >= SILENCE_TIMEOUT)
    {
        ResetSession();
        events |= GC_LINK_EVENT_TIMEOUT;
    }
    return events;
}
