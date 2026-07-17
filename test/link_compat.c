#include "global.h"
#include "link.h"
#include "test/test.h"
#include "config/link_compat.h"

static struct LinkPlayer CompatibleLinkPlayer(void)
{
    return (struct LinkPlayer)
    {
        .lp_field_2 = LINK_PROJECT_ID,
        .neverRead = LINK_PACKED_VERSIONS,
    };
}

static void SetCompatibleCableGroup(u8 playerCount)
{
    u32 i;

    gWirelessCommType = FALSE;
    gReceivedRemoteLinkPlayers = TRUE;
    gLinkStatus = playerCount << LINK_STAT_PLAYER_COUNT_SHIFT;
    for (i = 0; i < playerCount; i++)
        gLinkPlayers[i] = CompatibleLinkPlayer();
}

TEST("Link compatibility accepts the configured project and versions")
{
    struct LinkPlayer player = CompatibleLinkPlayer();

    EXPECT_EQ(GetLinkPlayerCompatibilityStatus(&player), LINK_COMPATIBILITY_OK);
}

TEST("Link compatibility rejects a different project")
{
    struct LinkPlayer player = CompatibleLinkPlayer();

    player.lp_field_2 ^= 1;
    EXPECT_EQ(GetLinkPlayerCompatibilityStatus(&player), LINK_COMPATIBILITY_PROJECT_MISMATCH);
}

TEST("Link compatibility rejects a different protocol version")
{
    struct LinkPlayer player = CompatibleLinkPlayer();
    u8 otherVersion = (LINK_COMPAT_VERSION ^ 1) & LINK_COMPAT_VERSION_MASK;

    player.neverRead = (otherVersion << LINK_COMPAT_VERSION_SHIFT) | LINK_DATA_LAYOUT_VERSION;
    EXPECT_EQ(GetLinkPlayerCompatibilityStatus(&player), LINK_COMPATIBILITY_VERSION_MISMATCH);
}

TEST("Link compatibility rejects a different data layout version")
{
    struct LinkPlayer player = CompatibleLinkPlayer();
    u8 otherVersion = (LINK_DATA_LAYOUT_VERSION ^ 1) & LINK_COMPAT_VERSION_MASK;

    player.neverRead = (LINK_COMPAT_VERSION << LINK_COMPAT_VERSION_SHIFT) | otherVersion;
    EXPECT_EQ(GetLinkPlayerCompatibilityStatus(&player), LINK_COMPATIBILITY_DATA_LAYOUT_MISMATCH);
}

TEST("Link compatibility applies only to configured cable protocols")
{
    EXPECT(DoesLinkTypeRequireCompatibility(LINKTYPE_TRADE));
    EXPECT(DoesLinkTypeRequireCompatibility(LINKTYPE_SINGLE_BATTLE));
    EXPECT(DoesLinkTypeRequireCompatibility(LINKTYPE_DOUBLE_BATTLE));
    EXPECT(DoesLinkTypeRequireCompatibility(LINKTYPE_MULTI_BATTLE));
    EXPECT(DoesLinkTypeRequireCompatibility(LINKTYPE_BATTLE_TOWER));
    EXPECT(DoesLinkTypeRequireCompatibility(LINKTYPE_CONTEST_EMODE));
    EXPECT(!DoesLinkTypeRequireCompatibility(LINKTYPE_MYSTERY_EVENT));
    EXPECT(!DoesLinkTypeRequireCompatibility(LINKTYPE_EREADER_EM));
}

TEST("Link compatibility rejects one mismatched player in a four player group")
{
    SetCompatibleCableGroup(4);
    EXPECT(AreAllLinkPlayersCompatible());

    gLinkPlayers[3].lp_field_2 ^= 1;
    EXPECT(!AreAllLinkPlayersCompatible());

    gReceivedRemoteLinkPlayers = FALSE;
    gLinkStatus = 0;
}

TEST("Link compatibility rejects a disconnected group")
{
    SetCompatibleCableGroup(2);
    gReceivedRemoteLinkPlayers = FALSE;

    EXPECT(!AreAllLinkPlayersCompatible());

    gLinkStatus = 0;
}
