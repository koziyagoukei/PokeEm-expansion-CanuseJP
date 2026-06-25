ALIGNED(4) static const u8 sText_EmptyString[] = _("");
ALIGNED(4) static const u8 sText_Colon[] = _(":");
ALIGNED(4) static const u8 sText_ID[] = _("{ID}");
ALIGNED(4) static const u8 sText_PleaseStartOver[] = _("さいしょから やりなおしてください。");
ALIGNED(4) static const u8 sText_WirelessSearchCanceled[] = _("ワイヤレスつうしんの\nけんさくを やめました。");
ALIGNED(4) static const u8 sText_AwaitingCommunucation2[] = _("ほかのプレイヤーからの\nつうしんを まっています。"); // Unused
ALIGNED(4) static const u8 sText_AwaitingCommunication[] = _("{STR_VAR_1}！\nつうしんを まっています。");
ALIGNED(4) static const u8 sText_AwaitingLinkPressStart[] = _("{STR_VAR_1}！ つうしんまち！\nじゅんびOKなら START。");
ALIGNED(4) static const u8 sJPText_SingleBattle[] = _("シングルバトルを かいさいする");
ALIGNED(4) static const u8 sJPText_DoubleBattle[] = _("ダブルバトルを かいさいする");
ALIGNED(4) static const u8 sJPText_MultiBattle[] = _("マルチバトルを かいさいする");
ALIGNED(4) static const u8 sJPText_TradePokemon[] = _("ポケモンこうかんを かいさいする");
ALIGNED(4) static const u8 sJPText_Chat[] = _("チャットを かいさいする");
ALIGNED(4) static const u8 sJPText_DistWonderCard[] = _("ふしぎなカードをくばる");
ALIGNED(4) static const u8 sJPText_DistWonderNews[] = _("ふしぎなニュースをくばる");
ALIGNED(4) static const u8 sJPText_DistMysteryEvent[] = _("ふしぎなできごとを かいさいする"); // Unused
ALIGNED(4) static const u8 sJPText_HoldPokemonJump[] = _("なわとびを かいさいする");
ALIGNED(4) static const u8 sJPText_HoldBerryCrush[] = _("きのみマッシャーを かいさいする");
ALIGNED(4) static const u8 sJPText_HoldBerryPicking[] = _("きのみどりを かいさいする");
ALIGNED(4) static const u8 sJPText_HoldSpinTrade[] = _("ぐるぐるこうかんを かいさいする");
ALIGNED(4) static const u8 sJPText_HoldSpinShop[] = _("ぐるぐるショップを かいさいする");

// Unused
static const u8 *const sJPLinkGroupActionTexts[] = {
    sJPText_SingleBattle,
    sJPText_DoubleBattle,
    sJPText_MultiBattle,
    sJPText_TradePokemon,
    sJPText_Chat,
    sJPText_DistWonderCard,
    sJPText_DistWonderNews,
    sJPText_DistWonderCard,
    sJPText_HoldPokemonJump,
    sJPText_HoldBerryCrush,
    sJPText_HoldBerryPicking,
    sJPText_HoldBerryPicking,
    sJPText_HoldSpinTrade,
    sJPText_HoldSpinShop
};

static const u8 sText_1PlayerNeeded[] = _("あと 1にん\nひつようです。");
static const u8 sText_2PlayersNeeded[] = _("あと 2にん\nひつようです。");
static const u8 sText_3PlayersNeeded[] = _("あと 3にん\nひつようです。");
static const u8 sText_4PlayersNeeded[] = _("あと 4にん\nひつようです。");
static const u8 sText_2PlayerMode[] = _("2にん\nモード");
static const u8 sText_3PlayerMode[] = _("3にん\nモード");
static const u8 sText_4PlayerMode[] = _("4にん\nモード");
static const u8 sText_5PlayerMode[] = _("5にん\nモード");

static const u8 *const sPlayersNeededOrModeTexts[][5] = {
    // 2 players required
    {
        sText_1PlayerNeeded,
        sText_2PlayerMode
    },
    // 4 players required
    {
        sText_3PlayersNeeded,
        sText_2PlayersNeeded,
        sText_1PlayerNeeded,
        sText_4PlayerMode
    },
    // 2-5 players required
    {
        sText_1PlayerNeeded,
        sText_2PlayerMode,
        sText_3PlayerMode,
        sText_4PlayerMode,
        sText_5PlayerMode
    },
    // 3-5 players required
    {
        sText_2PlayersNeeded,
        sText_1PlayerNeeded,
        sText_3PlayerMode,
        sText_4PlayerMode,
        sText_5PlayerMode
    },
    // 2-4 players required
    {
        sText_1PlayerNeeded,
        sText_2PlayerMode,
        sText_3PlayerMode,
        sText_4PlayerMode
    }
};

ALIGNED(4) static const u8 sText_BButtonCancel[] = _("{B_BUTTON}やめる");
ALIGNED(4) static const u8 sJPText_SearchingForParticipants[] = _("ため\nさんかしゃ ぼしゅうちゅう です？"); // Unused, may have been cut off
ALIGNED(4) static const u8 sText_PlayerContactedYouForXAccept[] = _("{STR_VAR_2}から\n{STR_VAR_1}の もうしこみ。うけますか？");
ALIGNED(4) static const u8 sText_PlayerContactedYouShareX[] = _("{STR_VAR_2}から れんらく。\n{STR_VAR_1}を わけますか？");
ALIGNED(4) static const u8 sText_PlayerContactedYouAddToMembers[] = _("{STR_VAR_2}から れんらく。\nメンバーに いれますか？");
ALIGNED(4) static const u8 sText_AreTheseMembersOK[] = _("{STR_VAR_1}！\nこのメンバーで いいですか？");
ALIGNED(4) static const u8 sText_CancelModeWithTheseMembers[] = _("このメンバーでの\n{STR_VAR_1}モードを やめますか？");
ALIGNED(4) static const u8 sText_AnOKWasSentToPlayer[] = _("{STR_VAR_1}に\nOKを おくりました。");
ALIGNED(4) static const u8 sText_OtherTrainerUnavailableNow[] = _("あいての トレーナーは\nいま つうしんできないようです...\p");
ALIGNED(4) static const u8 sText_CantTransmitTrainerTooFar[] = _("あいての トレーナーが\nとおすぎて つうしんできません。\p");
ALIGNED(4) static const u8 sText_TrainersNotReadyYet[] = _("ほかの トレーナーは\nまだ じゅんびできていません。\p");

static const u8 *const sCantTransmitToTrainerTexts[] = {
    [UR_TRADE_PLAYER_NOT_READY - 1]  = sText_CantTransmitTrainerTooFar,
    [UR_TRADE_PARTNER_NOT_READY - 1] = sText_TrainersNotReadyYet
};

ALIGNED(4) static const u8 sText_ModeWithTheseMembersWillBeCanceled[] = _("このメンバーでの\n{STR_VAR_1}モードを やめます。{PAUSE 60}");
ALIGNED(4) static const u8 sText_MemberNoLongerAvailable[] = _("つづけられない\nメンバーが います。\p");

static const u8 *const sPlayerUnavailableTexts[] = {
    sText_OtherTrainerUnavailableNow,
    sText_MemberNoLongerAvailable
};

ALIGNED(4) static const u8 sText_TrainerAppearsUnavailable[] = _("あいての トレーナーは\nつうしんできないようです...\p");
ALIGNED(4) static const u8 sText_PlayerSentBackOK[] =    _("{STR_VAR_1}から OKが かえってきた！");
ALIGNED(4) static const u8 sText_PlayerOKdRegistration[] = _("{STR_VAR_1}が メンバーとうろくを\nOKしました。");
ALIGNED(4) static const u8 sText_PlayerRepliedNo[] = _("{STR_VAR_1}から いいえの\nへんじが きました...\p");
ALIGNED(4) static const u8 sText_AwaitingOtherMembers[] = _("{STR_VAR_1}！\nほかのメンバーを まっています！");
ALIGNED(4) static const u8 sText_QuitBeingMember[] = _("メンバーを やめますか？");
ALIGNED(4) static const u8 sText_StoppedBeingMember[] = _("メンバーを やめました。\p");

static const u8 *const sPlayerDisconnectedTexts[] = {
    [RFU_STATUS_OK]                  = NULL,
    [RFU_STATUS_FATAL_ERROR]         = sText_MemberNoLongerAvailable,
    [RFU_STATUS_CONNECTION_ERROR]    = sText_TrainerAppearsUnavailable,
    [RFU_STATUS_CHILD_SEND_COMPLETE] = NULL,
    [RFU_STATUS_NEW_CHILD_DETECTED]  = NULL,
    [RFU_STATUS_JOIN_GROUP_OK]       = NULL,
    [RFU_STATUS_JOIN_GROUP_NO]       = sText_PlayerRepliedNo,
    [RFU_STATUS_WAIT_ACK_JOIN_GROUP] = NULL,
    [RFU_STATUS_LEAVE_GROUP_NOTICE]  = NULL,
    [RFU_STATUS_LEAVE_GROUP]         = sText_StoppedBeingMember
};

ALIGNED(4) static const u8 sText_WirelessLinkEstablished[] = _("ワイヤレスつうしんが\nつながりました。");
ALIGNED(4) static const u8 sText_WirelessLinkDropped[] = _("ワイヤレスつうしんが\nきれました...");
ALIGNED(4) static const u8 sText_LinkWithFriendDropped[] = _("ともだちとの つうしんが\nきれました...");
ALIGNED(4) static const u8 sText_PlayerRepliedNo2[] = _("{STR_VAR_1}から いいえの\nへんじが きました...");

static const u8 *const sLinkDroppedTexts[] = {
    [RFU_STATUS_OK]                  = NULL,
    [RFU_STATUS_FATAL_ERROR]         = sText_LinkWithFriendDropped,
    [RFU_STATUS_CONNECTION_ERROR]    = sText_LinkWithFriendDropped,
    [RFU_STATUS_CHILD_SEND_COMPLETE] = NULL,
    [RFU_STATUS_NEW_CHILD_DETECTED]  = NULL,
    [RFU_STATUS_JOIN_GROUP_OK]       = NULL,
    [RFU_STATUS_JOIN_GROUP_NO]       = sText_PlayerRepliedNo2,
    [RFU_STATUS_WAIT_ACK_JOIN_GROUP] = NULL,
    [RFU_STATUS_LEAVE_GROUP_NOTICE]  = NULL,
    [RFU_STATUS_LEAVE_GROUP]         = NULL
};

ALIGNED(4) static const u8 sText_DoYouWantXMode[] = _("{STR_VAR_2}モードに\nしますか？");
ALIGNED(4) static const u8 sText_DoYouWantXMode2[] = _("{STR_VAR_2}モードに\nしますか？");

// Unused
static const u8 *const sDoYouWantModeTexts[] = {
    sText_DoYouWantXMode,
    sText_DoYouWantXMode2
};

ALIGNED(4) static const u8 sText_CommunicatingPleaseWait[] = _("つうしんちゅう...\nおまちください。"); // Unused
ALIGNED(4) static const u8 sText_AwaitingPlayersResponseAboutTrade[] = _("{STR_VAR_1}からの\nこうかんの へんじまち...");
ALIGNED(4) static const u8 sText_Communicating[] = _("つうしんちゅう{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.\n"
                                                     "{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.");
ALIGNED(4) static const u8 sText_CommunicatingWithPlayer[] = _("{STR_VAR_1}と つうしんちゅう{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.\n"
                                                               "{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.");
ALIGNED(4) static const u8 sText_PleaseWaitAWhile[] = _("しばらく おまちください{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.\n"
                                                        "{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.");

static const u8 *const sCommunicatingWaitTexts[] = {
    sText_Communicating,
    sText_CommunicatingWithPlayer,
    sText_PleaseWaitAWhile
};

ALIGNED(4) static const u8 sText_HiDoSomethingMale[] = _("Hiya？ Is there something that you\nwanted to do？");
ALIGNED(4) static const u8 sText_HiDoSomethingFemale[] = _("Hello？\nWould you like to do something？");
ALIGNED(4) static const u8 sText_HiDoSomethingAgainMale[] = _("{STR_VAR_1}: Hiya, we meet again？\nWhat are you up for this time？");
ALIGNED(4) static const u8 sText_HiDoSomethingAgainFemale[] = _("{STR_VAR_1}: Oh？ {PLAYER}, hello？\nWould you like to do something？");

static const u8 *const sHiDoSomethingTexts[][GENDER_COUNT] = {
    {
        sText_HiDoSomethingMale,
        sText_HiDoSomethingFemale
    }, {
        sText_HiDoSomethingAgainMale,
        sText_HiDoSomethingAgainFemale
    }
};

ALIGNED(4) static const u8 sText_DoSomethingMale[] = _("Want to do something？");
ALIGNED(4) static const u8 sText_DoSomethingFemale[] = _("Would you like to do something？");
ALIGNED(4) static const u8 sText_DoSomethingAgainMale[] = _("{STR_VAR_1}: What would you like to\ndo now？");
ALIGNED(4) static const u8 sText_DoSomethingAgainFemale[] = _("{STR_VAR_1}: Want to do anything else？"); // Unused

// Unused
static const u8 *const sDoSomethingTexts[][GENDER_COUNT] = {
    {
        sText_DoSomethingMale,
        sText_DoSomethingFemale
    }, {
        sText_DoSomethingAgainMale,
        sText_DoSomethingAgainMale // was probably supposed to be sText_DoSomethingAgainFemale
    }
};

ALIGNED(4) static const u8 sText_SomebodyHasContactedYou[] = _("Somebody has contacted you.{PAUSE 60}");
ALIGNED(4) static const u8 sText_PlayerHasContactedYou[] = _("{STR_VAR_1} has contacted you.{PAUSE 60}");

static const u8 *const sPlayerContactedYouTexts[] = {
    sText_SomebodyHasContactedYou,
    sText_PlayerHasContactedYou
};

ALIGNED(4) static const u8 sText_AwaitingResponseFromTrainer[] = _("Awaiting a response from\nthe other TRAINER...");
ALIGNED(4) static const u8 sText_AwaitingResponseFromPlayer[] = _("Awaiting a response from\n{STR_VAR_1}...");

static const u8 *const sAwaitingResponseTexts[] = {
    sText_AwaitingResponseFromTrainer,
    sText_AwaitingResponseFromPlayer
};

ALIGNED(4) static const u8 sText_ShowTrainerCard[] = _("The other TRAINER showed\nyou their TRAINER CARD.\pWould you like to show your\nTRAINER CARD？");
ALIGNED(4) static const u8 sText_BattleChallenge[] = _("The other TRAINER challenges you\nto battle.\pWill you accept the battle\nchallenge？");
ALIGNED(4) static const u8 sText_ChatInvitation[] = _("The other TRAINER invites you\nto chat.\pWill you accept the chat\ninvitation？");
ALIGNED(4) static const u8 sText_OfferToTradeMon[] = _("There is an offer to trade your\nregistered Lv. {DYNAMIC 0} {DYNAMIC 1}\pin exchange for a\nLv. {DYNAMIC 2} {DYNAMIC 3}.\pWill you accept this trade\noffer？");
ALIGNED(4) static const u8 sText_OfferToTradeEgg[] = _("There is an offer to trade your\nregistered EGG.\lWill you accept this trade offer？");
ALIGNED(4) static const u8 sText_ChatDropped[] = _("The chat has been dropped.\p");
ALIGNED(4) static const u8 sText_OfferDeclined1[] = _("You declined the offer.\p");
ALIGNED(4) static const u8 sText_OfferDeclined2[] = _("You declined the offer.\p");
ALIGNED(4) static const u8 sText_ChatEnded[] = _("The chat was ended.\p");

// Unused
static const u8 *const sInvitationTexts[] = {
    sText_ShowTrainerCard,
    sText_BattleChallenge,
    sText_ChatInvitation,
    sText_OfferToTradeMon
};

ALIGNED(4) static const u8 sText_JoinChatMale[] = _("Oh, hey？ We're in a chat right now.\nWant to join us？");
ALIGNED(4) static const u8 sText_PlayerJoinChatMale[] = _("{STR_VAR_1}: Hey, {PLAYER}？\nWe're having a chat right now.\lWant to join us？");
ALIGNED(4) static const u8 sText_JoinChatFemale[] = _("Oh, hi？ We're having a chat now.\nWould you like to join us？");
ALIGNED(4) static const u8 sText_PlayerJoinChatFemale[] = _("{STR_VAR_1}: Oh, hi, {PLAYER}？\nWe're having a chat now.\lWould you like to join us？");

static const u8 *const sJoinChatTexts[][GENDER_COUNT] = {
    {
        sText_JoinChatMale,
        sText_JoinChatFemale
    }, {
        sText_PlayerJoinChatMale,
        sText_PlayerJoinChatFemale
    }
};

ALIGNED(4) static const u8 sText_TrainerAppearsBusy[] = _("......\nThe TRAINER appears to be busy...\p");
ALIGNED(4) static const u8 sText_WaitForBattleMale[] = _("A battle, huh？\nAll right, just give me some time.");
ALIGNED(4) static const u8 sText_WaitForChatMale[] = _("You want to chat, huh？\nSure, just wait a little.");
ALIGNED(4) static const u8 sText_ShowTrainerCardMale[] = _("Sure thing？ As my Greetings,\nhere's my TRAINER CARD.");
ALIGNED(4) static const u8 sText_WaitForBattleFemale[] = _("A battle？ Of course, but I need\ntime to get ready.");
ALIGNED(4) static const u8 sText_WaitForChatFemale[] = _("Did you want to chat？\nOkay, but please wait a moment.");
ALIGNED(4) static const u8 sText_ShowTrainerCardFemale[] = _("As my introduction, I'll show you\nmy TRAINER CARD.");

static const u8 *const sText_WaitOrShowCardTexts[GENDER_COUNT][4] = {
    {
        sText_WaitForBattleMale,
        sText_WaitForChatMale,
        NULL,
        sText_ShowTrainerCardMale
    }, {
        sText_WaitForBattleFemale,
        sText_WaitForChatFemale,
        NULL,
        sText_ShowTrainerCardFemale
    }
};

ALIGNED(4) static const u8 sText_WaitForChatMale2[] = _("You want to chat, huh？\nSure, just wait a little."); // Unused
ALIGNED(4) static const u8 sText_DoneWaitingBattleMale[] = _("Thanks for waiting？\nLet's get our battle started？{PAUSE 60}");
ALIGNED(4) static const u8 sText_DoneWaitingChatMale[] = _("All right？\nLet's chat？{PAUSE 60}");
ALIGNED(4) static const u8 sText_DoneWaitingBattleFemale[] = _("Sorry I made you wait？\nLet's get started？{PAUSE 60}");
ALIGNED(4) static const u8 sText_DoneWaitingChatFemale[] = _("Sorry I made you wait？\nLet's chat.{PAUSE 60}");
ALIGNED(4) static const u8 sText_TradeWillBeStarted[] = _("The trade will be started.{PAUSE 60}");
ALIGNED(4) static const u8 sText_BattleWillBeStarted[] = _("The battle will be started.{PAUSE 60}");
ALIGNED(4) static const u8 sText_EnteringChat[] = _("Entering the chat...{PAUSE 60}");

static const u8 *const sStartActivityTexts[][GENDER_COUNT][3] = {
    {
        {
            sText_BattleWillBeStarted,
            sText_EnteringChat,
            sText_TradeWillBeStarted
        }, {
            sText_BattleWillBeStarted,
            sText_EnteringChat,
            sText_TradeWillBeStarted
        }
    }, {
        {
            sText_DoneWaitingBattleMale,
            sText_DoneWaitingChatMale,
            sText_TradeWillBeStarted
        }, {
            sText_DoneWaitingBattleFemale,
            sText_DoneWaitingChatFemale,
            sText_TradeWillBeStarted
        }
    }
};

ALIGNED(4) static const u8 sText_BattleDeclinedMale[] = _("Sorry？ My POKEMON don't seem to\nbe feeling too well right now.\lLet me battle you another time.\p");
ALIGNED(4) static const u8 sText_BattleDeclinedFemale[] = _("I'm terribly sorry, but my POKEMON\naren't feeling well...\pLet's battle another time.\p");

static const u8 *const sBattleDeclinedTexts[GENDER_COUNT] = {
    sText_BattleDeclinedMale,
    sText_BattleDeclinedFemale
};

ALIGNED(4) static const u8 sText_ShowTrainerCardDeclinedMale[] = _("Huh？ My TRAINER CARD...\nWhere'd it go now？\lSorry？ I'll show you another time？\p");
ALIGNED(4) static const u8 sText_ShowTrainerCardDeclinedFemale[] = _("Oh？ Now where did I put my\nTRAINER CARD？...\lSorry？ I'll show you later？\p");

static const u8 *const sShowTrainerCardDeclinedTexts[GENDER_COUNT] = {
    sText_ShowTrainerCardDeclinedMale,
    sText_ShowTrainerCardDeclinedFemale
};

ALIGNED(4) static const u8 sText_IfYouWantToDoSomethingMale[] = _("If you want to do something with\nme, just give me a shout？\p");
ALIGNED(4) static const u8 sText_IfYouWantToDoSomethingFemale[] = _("If you want to do something with\nme, don't be shy.\p");

static const u8 *const sIfYouWantToDoSomethingTexts[GENDER_COUNT] = {
    sText_IfYouWantToDoSomethingMale,
    sText_IfYouWantToDoSomethingFemale
};

ALIGNED(4) static const u8 sText_TrainerBattleBusy[] = _("Whoops？ Sorry, but I have to do\nsomething else.\lAnother time, okay？\p");
ALIGNED(4) static const u8 sText_NeedTwoMonsOfLevel30OrLower1[] = _("If you want to battle, you need\ntwo POKEMON that are below\lLv. 30.\p");
ALIGNED(4) static const u8 sText_NeedTwoMonsOfLevel30OrLower2[] = _("For a battle, you need two\nPOKEMON that are below Lv. 30.\p");

ALIGNED(4) static const u8 sText_DeclineChatMale[] = _("Oh, all right.\nCome see me anytime, okay？\p");
ALIGNED(4) static const u8 stext_DeclineChatFemale[] = _("Oh...\nPlease come by anytime.\p");

// Response from partner when player declines chat
static const u8 *const sDeclineChatTexts[GENDER_COUNT] = {
    sText_DeclineChatMale,
    stext_DeclineChatFemale
};

ALIGNED(4) static const u8 sText_ChatDeclinedMale[] = _("Oh, sorry？\nI just can't right this instant.\lLet's chat another time.\p");
ALIGNED(4) static const u8 sText_ChatDeclinedFemale[] = _("Oh, I'm sorry.\nI have too much to do right now.\lLet's chat some other time.\p");

// Response from partner when they decline chat
static const u8 *const sChatDeclinedTexts[GENDER_COUNT] = {
    sText_ChatDeclinedMale,
    sText_ChatDeclinedFemale
};

ALIGNED(4) static const u8 sText_YoureToughMale[] = _("Whoa？\nI can tell you're pretty tough？\p");
ALIGNED(4) static const u8 sText_UsedGoodMoveMale[] = _("You used that move？\nThat's good strategy？\p");
ALIGNED(4) static const u8 sText_BattleSurpriseMale[] = _("Way to go？\nThat was an eye？opener？\p");
ALIGNED(4) static const u8 sText_SwitchedMonsMale[] = _("Oh？ How could you use that\nPOKEMON in that situation？\p");
ALIGNED(4) static const u8 sText_YoureToughFemale[] = _("That POKEMON...\nIt's been raised really well？\p");
ALIGNED(4) static const u8 sText_UsedGoodMoveFemale[] = _("That's it？\nThis is the right move now？\p");
ALIGNED(4) static const u8 sText_BattleSurpriseFemale[] = _("That's awesome？\nYou can battle that way？\p");
ALIGNED(4) static const u8 sText_SwitchedMonsFemale[] = _("You have exquisite timing for\nswitching POKEMON？\p");

static const u8 *const sBattleReactionTexts[GENDER_COUNT][4] = {
    {
        sText_YoureToughMale,
        sText_UsedGoodMoveMale,
        sText_BattleSurpriseMale,
        sText_SwitchedMonsMale
    },
    {
        sText_YoureToughFemale,
        sText_UsedGoodMoveFemale,
        sText_BattleSurpriseFemale,
        sText_SwitchedMonsFemale
    }
};

ALIGNED(4) static const u8 sText_LearnedSomethingMale[] = _("Oh, I see？\nThis is educational？\p");
ALIGNED(4) static const u8 sText_ThatsFunnyMale[] = _("Don't say anything funny anymore？\nI'm sore from laughing？\p");
ALIGNED(4) static const u8 sText_RandomChatMale1[] = _("Oh？\nSomething like that happened.\p");
ALIGNED(4) static const u8 sText_RandomChatMale2[] = _("Hmhm... What？\nSo is this what you're saying？\p");
ALIGNED(4) static const u8 sText_LearnedSomethingFemale[] = _("Is that right？\nI didn't know that.\p");
ALIGNED(4) static const u8 sText_ThatsFunnyFemale[] = _("Ahaha？\nWhat is that about？\p");
ALIGNED(4) static const u8 sText_RandomChatFemale1[] = _("Yes, that's exactly it？\nThat's what I meant.\p");
ALIGNED(4) static const u8 sText_RandomChatFemale2[] = _("In other words...\nYes？ That's right？\p");

static const u8 *const sChatReactionTexts[GENDER_COUNT][4] = {
    {
        sText_LearnedSomethingMale,
        sText_ThatsFunnyMale,
        sText_RandomChatMale1,
        sText_RandomChatMale2
    },
    {
        sText_LearnedSomethingFemale,
        sText_ThatsFunnyFemale,
        sText_RandomChatFemale1,
        sText_RandomChatFemale2
    }
};

ALIGNED(4) static const u8 sText_ShowedTrainerCardMale1[] = _("I'm just showing my TRAINER CARD\nas my way of greeting.\p");
ALIGNED(4) static const u8 sText_ShowedTrainerCardMale2[] = _("I hope I get to know you better？\p");
ALIGNED(4) static const u8 sText_ShowedTrainerCardFemale1[] = _("We're showing each other our\nTRAINER CARDS to get acquainted.\p");
ALIGNED(4) static const u8 sText_ShowedTrainerCardFemale2[] = _("Glad to meet you.\nPlease don't be a stranger？\p");

static const u8 *const sTrainerCardReactionTexts[GENDER_COUNT][2] = {
    {
        sText_ShowedTrainerCardMale1,
        sText_ShowedTrainerCardMale2
    },
    {
        sText_ShowedTrainerCardFemale1,
        sText_ShowedTrainerCardFemale2
    }
};

ALIGNED(4) static const u8 sText_MaleTraded1[] = _("Yeahah？\nI really wanted this POKEMON？\p");
ALIGNED(4) static const u8 sText_MaleTraded2[] = _("Finally, a trade got me that\nPOKEMON I'd wanted a long time.\p");
ALIGNED(4) static const u8 sText_FemaleTraded1[] = _("I'm trading POKEMON right now.\p");
ALIGNED(4) static const u8 sText_FemaleTraded2[] = _("I finally got that POKEMON I\nwanted in a trade？\p");

static const u8 *const sTradeReactionTexts[GENDER_COUNT][4] = {
    {
        sText_MaleTraded1,
        sText_MaleTraded2
    },
    {
        sText_FemaleTraded1,
        sText_FemaleTraded2
    }
};

ALIGNED(4) static const u8 sText_XCheckedTradingBoard[] = _("{STR_VAR_1} checked the\nTRADING BOARD.\p");
ALIGNED(4) static const u8 sText_RegisterMonAtTradingBoard[] = _("Welcome to the TRADING BOARD.\pYou may register your POKEMON\nand offer it up for a trade.\pWould you like to register one of\nyour POKEMON？");
ALIGNED(4) static const u8 sText_TradingBoardInfo[] = _("This TRADING BOARD is used for\n"
                                                        "offering a POKEMON for a trade.\p"
                                                        "All you need to do is register a\n"
                                                        "POKEMON for a trade.\p"
                                                        "Another TRAINER may offer a party\n"
                                                        "POKEMON in return for the trade.\p"
                                                        "We hope you will register POKEMON\n"
                                                        "and trade them with many, many\l"
                                                        "other TRAINERS.\p"
                                                        "Would you like to register one of\nyour POKEMON？");
ALIGNED(4) static const u8 sText_ThankYouForRegistering[] = _("We have registered your POKEMON for\ntrade on the TRADING BOARD.\pThank you for using this service？\p"); // unused
ALIGNED(4) static const u8 sText_NobodyHasRegistered[] = _("Nobody has registered any POKEMON\nfor trade on the TRADING BOARD.\p\n"); // unused
ALIGNED(4) static const u8 sText_ChooseRequestedMonType[] = _("Please choose the type of POKEMON\nthat you would like in the trade.\n");
ALIGNED(4) static const u8 sText_WhichMonWillYouOffer[] = _("Which of your party POKEMON will\nyou offer in trade？\p");
ALIGNED(4) static const u8 sText_RegistrationCanceled[] = _("Registration has been canceled.\p");
ALIGNED(4) static const u8 sText_RegistrationCompleted[] = _("Registration has been completed.\p");
ALIGNED(4) static const u8 sText_TradeCanceled[] = _("The trade has been canceled.\p");
ALIGNED(4) static const u8 sText_CancelRegistrationOfMon[] = _("Cancel the registration of your\nLv. {STR_VAR_2} {STR_VAR_1}？");
ALIGNED(4) static const u8 sText_CancelRegistrationOfEgg[] = _("Cancel the registration of your\nEGG？");
ALIGNED(4) static const u8 sText_RegistrationCanceled2[] = _("The registration has been canceled.\p");
ALIGNED(4) static const u8 sText_TradeTrainersWillBeListed[] = _("TRAINERS wishing to make a trade\nwill be listed."); // unused
ALIGNED(4) static const u8 sText_ChooseTrainerToTradeWith2[] = _("Please choose the TRAINER with whom\nyou would like to trade POKEMON."); // unused
ALIGNED(4) static const u8 sText_AskTrainerToMakeTrade[] = _("Would you like to ask {STR_VAR_1} to\nmake a trade？");
ALIGNED(4) static const u8 sText_AwaitingResponseFromTrainer2[] = _("Awaiting a response from\nthe other TRAINER..."); // unused
ALIGNED(4) static const u8 sText_NotRegisteredAMonForTrade[] = _("You have not registered a POKEMON\nfor trading.\p"); // unused
ALIGNED(4) static const u8 sText_DontHaveTypeTrainerWants[] = _("You don't have a {STR_VAR_2}？type\nPOKEMON that {STR_VAR_1} wants.\p");
ALIGNED(4) static const u8 sText_DontHaveEggTrainerWants[] = _("You don't have an EGG that\n{STR_VAR_1} wants.\p");
ALIGNED(4) static const u8 sText_PlayerCantTradeForYourMon[] = _("{STR_VAR_1} can't make a trade for\nyour POKEMON right now.\p");
ALIGNED(4) static const u8 sText_CantTradeForPartnersMon[] = _("You can't make a trade for\n{STR_VAR_1}'s POKEMON right now.\p");

// Unused
static const u8 *const sCantTradeMonTexts[] = {
    sText_PlayerCantTradeForYourMon,
    sText_CantTradeForPartnersMon
};

ALIGNED(4) static const u8 sText_TradeOfferRejected[] = _("こうかんの もうしこみを\nことわられました。\p");
ALIGNED(4) static const u8 sText_EggTrade[] = _("タマゴこうかん");
ALIGNED(4) static const u8 sText_ChooseJoinCancel[] = _("{DPAD_UPDOWN}えらぶ  {A_BUTTON}さんか  {B_BUTTON}やめる");
ALIGNED(4) static const u8 sText_ChooseTrainer[] = _("トレーナーを えらんでください。");
ALIGNED(4) static const u8 sText_ChooseTrainerSingleBattle[] = _("シングルバトルの\nトレーナーを えらんでください。");
ALIGNED(4) static const u8 sText_ChooseTrainerDoubleBattle[] = _("ダブルバトルの\nトレーナーを えらんでください。");
ALIGNED(4) static const u8 sText_ChooseLeaderMultiBattle[] = _("マルチバトルの\nリーダーを えらんでください。");
ALIGNED(4) static const u8 sText_ChooseTrainerToTradeWith[] = _("こうかんする トレーナーを\nえらんでください。");
ALIGNED(4) static const u8 sText_ChooseTrainerToShareWonderCards[] = _("ふしぎなカードを くばる\nトレーナーを えらんでください。");
ALIGNED(4) static const u8 sText_ChooseTrainerToShareWonderNews[] = _("ふしぎなニュースを くばる\nトレーナーを えらんでください。");
ALIGNED(4) static const u8 sText_ChooseLeaderPokemonJump[] = _("ミニポケモンジャンプ！\nリーダーを えらんでください。");
ALIGNED(4) static const u8 sText_ChooseLeaderBerryCrush[] = _("きのみクラッシュ！\nリーダーを えらんでください。");
ALIGNED(4) static const u8 sText_ChooseLeaderBerryPicking[] = _("ドードリオの きのみどり！\nリーダーを えらんでください。");
ALIGNED(4) static const u8 sText_ChooseLeaderBerryBlender[] = _("きのみブレンダー！\nリーダーを えらんでください。");
ALIGNED(4) static const u8 sText_ChooseLeaderRecordCorner[] = _("きろくコーナー！\nリーダーを えらんでください。");
ALIGNED(4) static const u8 sText_ChooseLeaderCoolContest[] = _("かっこよさコンテスト！\nリーダーを えらんでください。");
ALIGNED(4) static const u8 sText_ChooseLeaderBeautyContest[] = _("うつくしさコンテスト！\nリーダーを えらんでください。");
ALIGNED(4) static const u8 sText_ChooseLeaderCuteContest[] = _("かわいさコンテスト！\nリーダーを えらんでください。");
ALIGNED(4) static const u8 sText_ChooseLeaderSmartContest[] = _("かしこさコンテスト！\nリーダーを えらんでください。");
ALIGNED(4) static const u8 sText_ChooseLeaderToughContest[] = _("たくましさコンテスト！\nリーダーを えらんでください。");
ALIGNED(4) static const u8 sText_ChooseLeaderBattleTowerLv50[] = _("バトルタワー Lv.50！\nリーダーを えらんでください。");
ALIGNED(4) static const u8 sText_ChooseLeaderBattleTowerOpenLv[] = _("バトルタワー オープンレベル！\nリーダーを えらんでください。");

static const u8 *const sChooseTrainerTexts[NUM_LINK_GROUP_TYPES] =
{
    [LINK_GROUP_SINGLE_BATTLE]     = sText_ChooseTrainerSingleBattle,
    [LINK_GROUP_DOUBLE_BATTLE]     = sText_ChooseTrainerDoubleBattle,
    [LINK_GROUP_MULTI_BATTLE]      = sText_ChooseLeaderMultiBattle,
    [LINK_GROUP_TRADE]             = sText_ChooseTrainerToTradeWith,
    [LINK_GROUP_POKEMON_JUMP]      = sText_ChooseLeaderPokemonJump,
    [LINK_GROUP_BERRY_CRUSH]       = sText_ChooseLeaderBerryCrush,
    [LINK_GROUP_BERRY_PICKING]     = sText_ChooseLeaderBerryPicking,
    [LINK_GROUP_WONDER_CARD]       = sText_ChooseTrainerToShareWonderCards,
    [LINK_GROUP_WONDER_NEWS]       = sText_ChooseTrainerToShareWonderNews,
    [LINK_GROUP_UNION_ROOM_RESUME] = NULL,
    [LINK_GROUP_UNION_ROOM_INIT]   = NULL,
    [LINK_GROUP_UNK_11]            = NULL,
    [LINK_GROUP_RECORD_CORNER]     = sText_ChooseLeaderRecordCorner,
    [LINK_GROUP_BERRY_BLENDER]     = sText_ChooseLeaderBerryBlender,
    [LINK_GROUP_UNK_14]            = NULL,
    [LINK_GROUP_COOL_CONTEST]      = sText_ChooseLeaderCoolContest,
    [LINK_GROUP_BEAUTY_CONTEST]    = sText_ChooseLeaderBeautyContest,
    [LINK_GROUP_CUTE_CONTEST]      = sText_ChooseLeaderCuteContest,
    [LINK_GROUP_SMART_CONTEST]     = sText_ChooseLeaderSmartContest,
    [LINK_GROUP_TOUGH_CONTEST]     = sText_ChooseLeaderToughContest,
    [LINK_GROUP_BATTLE_TOWER]      = sText_ChooseLeaderBattleTowerLv50,
    [LINK_GROUP_BATTLE_TOWER_OPEN] = sText_ChooseLeaderBattleTowerOpenLv
};

ALIGNED(4) static const u8 sText_SearchingForWirelessSystemWait[] = _("ワイヤレスつうしんを\nけんさくちゅうです...");
ALIGNED(4) static const u8 sText_MustHaveTwoMonsForDoubleBattle[] = _("ダブルバトルには\nポケモンが 2ひき ひつようです。\p"); // Unused
ALIGNED(4) static const u8 sText_AwaitingPlayersResponse[] = _("{STR_VAR_1}からの へんじまち...");
ALIGNED(4) static const u8 sText_PlayerHasBeenAskedToRegisterYouPleaseWait[] = _("{STR_VAR_1}に メンバーとうろくを\nたのみました。 おまちください。");
ALIGNED(4) static const u8 sText_AwaitingResponseFromWirelessSystem[] = _("ワイヤレスつうしんからの\nへんじを まっています。");
ALIGNED(4) static const u8 sText_PleaseWaitForOtherTrainersToGather[] = _("ほかの トレーナーが\nあつまるまで おまちください。"); // Unused
ALIGNED(4) static const u8 sText_NoCardsSharedRightNow[] = _("いま くばられている\nカードは ありません。");
ALIGNED(4) static const u8 sText_NoNewsSharedRightNow[] = _("いま くばられている\nニュースは ありません。");

static const u8 *const sNoWonderSharedTexts[] = {
    sText_NoCardsSharedRightNow,
    sText_NoNewsSharedRightNow
};

ALIGNED(4) static const u8 sText_Battle[] = _("バトル");
ALIGNED(4) static const u8 sText_Chat2[] = _("チャット");
ALIGNED(4) static const u8 sText_Greetings[] = _("あいさつ");
ALIGNED(4) static const u8 sText_Exit[] = _("でる");
ALIGNED(4) static const u8 sText_Exit2[] = _("でる");
ALIGNED(4) static const u8 sText_Info[] = _("せつめい");
ALIGNED(4) static const u8 sText_NameWantedOfferLv[] = _("なまえ{CLEAR_TO 60}ほしい{CLEAR_TO 110}だす{CLEAR_TO 198}Lv.");
ALIGNED(4) static const u8 sText_SingleBattle[] = _("シングルバトル");
ALIGNED(4) static const u8 sText_DoubleBattle[] = _("ダブルバトル");
ALIGNED(4) static const u8 sText_MultiBattle[] = _("マルチバトル");
ALIGNED(4) static const u8 sText_PokemonTrades[] = _("ポケモンこうかん");
ALIGNED(4) static const u8 sText_Chat[] = _("チャット");
ALIGNED(4) static const u8 sText_Cards[] = _("カード");
ALIGNED(4) static const u8 sText_WonderCards[] = _("ふしぎなカード");
ALIGNED(4) static const u8 sText_WonderNews[] = _("ふしぎなニュース");
ALIGNED(4) static const u8 sText_PokemonJump[] = _("ポケモンジャンプ");
ALIGNED(4) static const u8 sText_BerryCrush[] = _("きのみクラッシュ");
ALIGNED(4) static const u8 sText_BerryPicking[] = _("きのみどり");
ALIGNED(4) static const u8 sText_Search[] = _("さがす");
ALIGNED(4) static const u8 sText_BerryBlender[] = _("きのみブレンダー");
ALIGNED(4) static const u8 sText_RecordCorner[] = _("きろくコーナー");
ALIGNED(4) static const u8 sText_CoolContest[] = _("かっこよさコンテスト");
ALIGNED(4) static const u8 sText_BeautyContest[] = _("うつくしさコンテスト");
ALIGNED(4) static const u8 sText_CuteContest[] = _("かわいさコンテスト");
ALIGNED(4) static const u8 sText_SmartContest[] = _("かしこさコンテスト");
ALIGNED(4) static const u8 sText_ToughContest[] = _("たくましさコンテスト");
ALIGNED(4) static const u8 sText_BattleTowerLv50[] = _("バトルタワー Lv. 50");
ALIGNED(4) static const u8 sText_BattleTowerOpenLv[] = _("バトルタワー オープンレベル");
ALIGNED(4) static const u8 sText_ItsNormalCard[] = _("ふつうのカードです。");
ALIGNED(4) static const u8 sText_ItsBronzeCard[] = _("ブロンズカードです！");
ALIGNED(4) static const u8 sText_ItsCopperCard[] = _("カッパーカードです！");
ALIGNED(4) static const u8 sText_ItsSilverCard[] = _("シルバーカードです！");
ALIGNED(4) static const u8 sText_ItsGoldCard[] = _("ゴールドカードです！");

static const u8 *const sCardColorTexts[] = {
    sText_ItsNormalCard,
    sText_ItsBronzeCard,
    sText_ItsCopperCard,
    sText_ItsSilverCard,
    sText_ItsGoldCard
};

ALIGNED(4) static const u8 sText_TrainerCardInfoPage1[] = _("{DYNAMIC 0} {DYNAMIC 1}の\nトレーナーカード...\l{DYNAMIC 2}\pポケモンずかん: {DYNAMIC 3}\nじかん:    {DYNAMIC 4}:{DYNAMIC 5}\p");
ALIGNED(4) static const u8 sText_TrainerCardInfoPage2[] = _("バトル: かち: {DYNAMIC 0}  まけ: {DYNAMIC 2}\nこうかん: {DYNAMIC 3}\p{DYNAMIC 4} {DYNAMIC 5}\n{DYNAMIC 6} {DYNAMIC 7}\p");
ALIGNED(4) static const u8 sText_GladToMeetYouMale[] = _("{DYNAMIC 1}: あえて よかった！{PAUSE 60}");
ALIGNED(4) static const u8 sText_GladToMeetYouFemale[] = _("{DYNAMIC 1}: よろしくね！{PAUSE 60}");

static const u8 *const sGladToMeetYouTexts[GENDER_COUNT] = {
    sText_GladToMeetYouMale,
    sText_GladToMeetYouFemale
};

ALIGNED(4) static const u8 sText_FinishedCheckingPlayersTrainerCard[] = _("{DYNAMIC 1}の\nトレーナーカードを みおわりました。{PAUSE 60}");

static const u8 *const sLinkGroupActivityNameTexts[] = {
    [ACTIVITY_NONE]              = sText_EmptyString,
    [ACTIVITY_BATTLE_SINGLE]     = sText_SingleBattle,
    [ACTIVITY_BATTLE_DOUBLE]     = sText_DoubleBattle,
    [ACTIVITY_BATTLE_MULTI]      = sText_MultiBattle,
    [ACTIVITY_TRADE]             = sText_PokemonTrades,
    [ACTIVITY_CHAT]              = sText_Chat,
    [ACTIVITY_WONDER_CARD_DUP]   = sText_WonderCards,
    [ACTIVITY_WONDER_NEWS_DUP]   = sText_WonderNews,
    [ACTIVITY_CARD]              = sText_Cards,
    [ACTIVITY_POKEMON_JUMP]      = sText_PokemonJump,
    [ACTIVITY_BERRY_CRUSH]       = sText_BerryCrush,
    [ACTIVITY_BERRY_PICK]        = sText_BerryPicking,
    [ACTIVITY_SEARCH]            = sText_Search,
    [ACTIVITY_SPIN_TRADE]        = sText_EmptyString,
    [ACTIVITY_BATTLE_TOWER_OPEN] = sText_BattleTowerOpenLv,
    [ACTIVITY_RECORD_CORNER]     = sText_RecordCorner,
    [ACTIVITY_BERRY_BLENDER]     = sText_BerryBlender,
    [ACTIVITY_ACCEPT]            = sText_EmptyString,
    [ACTIVITY_DECLINE]           = sText_EmptyString,
    [ACTIVITY_NPCTALK]           = sText_EmptyString,
    [ACTIVITY_PLYRTALK]          = sText_EmptyString,
    [ACTIVITY_WONDER_CARD]       = sText_WonderCards,
    [ACTIVITY_WONDER_NEWS]       = sText_WonderNews,
    [ACTIVITY_CONTEST_COOL]      = sText_CoolContest,
    [ACTIVITY_CONTEST_BEAUTY]    = sText_BeautyContest,
    [ACTIVITY_CONTEST_CUTE]      = sText_CuteContest,
    [ACTIVITY_CONTEST_SMART]     = sText_SmartContest,
    [ACTIVITY_CONTEST_TOUGH]     = sText_ToughContest,
    [ACTIVITY_BATTLE_TOWER]      = sText_BattleTowerLv50
};

static const struct WindowTemplate sWindowTemplate_BButtonCancel = {
    .bg = 0,
    .tilemapLeft = 0,
    .tilemapTop = 0,
    .width = 30,
    .height = 2,
    .paletteNum = 15,
    .baseBlock = 0x0008
};

// Minimum and maximum number of players for a link group
// A minimum of 0 means the min and max are equal
#define LINK_GROUP_CAPACITY(min, max) (((min) << 12) | ((max) << 8))
#define GROUP_MAX(capacity) (capacity & 0x0F)
#define GROUP_MIN(capacity) (capacity >> 4)
#define GROUP_MIN2(capacity) (capacity & 0xF0) // Unnecessary to have both, but needed to match

static const u32 sLinkGroupToActivityAndCapacity[NUM_LINK_GROUP_TYPES] = {
    [LINK_GROUP_SINGLE_BATTLE]     = ACTIVITY_BATTLE_SINGLE     | LINK_GROUP_CAPACITY(0, 2),
    [LINK_GROUP_DOUBLE_BATTLE]     = ACTIVITY_BATTLE_DOUBLE     | LINK_GROUP_CAPACITY(0, 2),
    [LINK_GROUP_MULTI_BATTLE]      = ACTIVITY_BATTLE_MULTI      | LINK_GROUP_CAPACITY(0, 4),
    [LINK_GROUP_TRADE]             = ACTIVITY_TRADE             | LINK_GROUP_CAPACITY(0, 2),
    [LINK_GROUP_POKEMON_JUMP]      = ACTIVITY_POKEMON_JUMP      | LINK_GROUP_CAPACITY(2, 5),
    [LINK_GROUP_BERRY_CRUSH]       = ACTIVITY_BERRY_CRUSH       | LINK_GROUP_CAPACITY(2, 5),
    [LINK_GROUP_BERRY_PICKING]     = ACTIVITY_BERRY_PICK        | LINK_GROUP_CAPACITY(3, 5),
    [LINK_GROUP_WONDER_CARD]       = ACTIVITY_NONE              | LINK_GROUP_CAPACITY(0, 0),
    [LINK_GROUP_WONDER_NEWS]       = ACTIVITY_NONE              | LINK_GROUP_CAPACITY(0, 0),
    [LINK_GROUP_UNION_ROOM_RESUME] = ACTIVITY_NONE              | LINK_GROUP_CAPACITY(0, 0),
    [LINK_GROUP_UNION_ROOM_INIT]   = ACTIVITY_NONE              | LINK_GROUP_CAPACITY(0, 0),
    [LINK_GROUP_UNK_11]            = ACTIVITY_NONE              | LINK_GROUP_CAPACITY(0, 0),
    [LINK_GROUP_RECORD_CORNER]     = ACTIVITY_RECORD_CORNER     | LINK_GROUP_CAPACITY(2, 4),
    [LINK_GROUP_BERRY_BLENDER]     = ACTIVITY_BERRY_BLENDER     | LINK_GROUP_CAPACITY(2, 4),
    [LINK_GROUP_UNK_14]            = ACTIVITY_NONE              | LINK_GROUP_CAPACITY(0, 0),
    [LINK_GROUP_COOL_CONTEST]      = ACTIVITY_CONTEST_COOL      | LINK_GROUP_CAPACITY(2, 4),
    [LINK_GROUP_BEAUTY_CONTEST]    = ACTIVITY_CONTEST_BEAUTY    | LINK_GROUP_CAPACITY(2, 4),
    [LINK_GROUP_CUTE_CONTEST]      = ACTIVITY_CONTEST_CUTE      | LINK_GROUP_CAPACITY(2, 4),
    [LINK_GROUP_SMART_CONTEST]     = ACTIVITY_CONTEST_SMART     | LINK_GROUP_CAPACITY(2, 4),
    [LINK_GROUP_TOUGH_CONTEST]     = ACTIVITY_CONTEST_TOUGH     | LINK_GROUP_CAPACITY(2, 4),
    [LINK_GROUP_BATTLE_TOWER]      = ACTIVITY_BATTLE_TOWER      | LINK_GROUP_CAPACITY(0, 2),
    [LINK_GROUP_BATTLE_TOWER_OPEN] = ACTIVITY_BATTLE_TOWER_OPEN | LINK_GROUP_CAPACITY(0, 2)
};

static const struct WindowTemplate sWindowTemplate_PlayerList = {
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 3,
    .width = 13,
    .height = 8,
    .paletteNum = 15,
    .baseBlock = 0x0044
};

static const struct WindowTemplate sWindowTemplate_5PlayerList = {
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 3,
    .width = 13,
    .height = 10,
    .paletteNum = 15,
    .baseBlock = 0x0044
};

static const struct WindowTemplate sWindowTemplate_NumPlayerMode = {
    .bg = 0,
    .tilemapLeft = 16,
    .tilemapTop = 3,
    .width = 7,
    .height = 4,
    .paletteNum = 15,
    .baseBlock = 0x00c6
};

static const struct ListMenuItem sPossibleGroupMembersListMenuItems[] = {
    { sText_EmptyString, 0 },
    { sText_EmptyString, 1 },
    { sText_EmptyString, 2 },
    { sText_EmptyString, 3 },
    { sText_EmptyString, 4 }
};

static const struct ListMenuTemplate sListMenuTemplate_PossibleGroupMembers = {
    .items = sPossibleGroupMembersListMenuItems,
    .moveCursorFunc = NULL,
    .itemPrintFunc = ItemPrintFunc_PossibleGroupMembers,
    .totalItems = ARRAY_COUNT(sPossibleGroupMembersListMenuItems),
    .maxShowed = 5,
    .windowId = 0,
    .header_X = 0,
    .item_X = 0,
    .cursor_X = 0,
    .upText_Y = 1,
    .cursorPal = 2,
    .fillValue = 1,
    .cursorShadowPal = 3,
    .lettersSpacing = 0,
    .itemVerticalPadding = 0,
    .scrollMultiple = LIST_NO_MULTIPLE_SCROLL,
    .fontId = FONT_NORMAL,
    .cursorKind = CURSOR_INVISIBLE
};

static const struct WindowTemplate sWindowTemplate_GroupList = {
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 3,
    .width = 17,
    .height = 10,
    .paletteNum = 15,
    .baseBlock = 0x0044
};

static const struct WindowTemplate sWindowTemplate_PlayerNameAndId = {
    .bg = 0,
    .tilemapLeft = 20,
    .tilemapTop = 3,
    .width = 7,
    .height = 4,
    .paletteNum = 15,
    .baseBlock = 0x00ee
};

static const struct ListMenuItem sUnionRoomGroupsMenuItems[] = {
    { sText_EmptyString,  0 },
    { sText_EmptyString,  1 },
    { sText_EmptyString,  2 },
    { sText_EmptyString,  3 },
    { sText_EmptyString,  4 },
    { sText_EmptyString,  5 },
    { sText_EmptyString,  6 },
    { sText_EmptyString,  7 },
    { sText_EmptyString,  8 },
    { sText_EmptyString,  9 },
    { sText_EmptyString, 10 },
    { sText_EmptyString, 11 },
    { sText_EmptyString, 12 },
    { sText_EmptyString, 13 },
    { sText_EmptyString, 14 },
    { sText_EmptyString, 15 }
};

static const struct ListMenuTemplate sListMenuTemplate_UnionRoomGroups = {
    .items = sUnionRoomGroupsMenuItems,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .itemPrintFunc = ListMenuItemPrintFunc_UnionRoomGroups,
    .totalItems = ARRAY_COUNT(sUnionRoomGroupsMenuItems),
    .maxShowed = 5,
    .windowId = 0,
    .header_X = 0,
    .item_X = 8,
    .cursor_X = 0,
    .upText_Y = 1,
    .cursorPal = 2,
    .fillValue = 1,
    .cursorShadowPal = 3,
    .lettersSpacing = 0,
    .itemVerticalPadding = 0,
    .scrollMultiple = LIST_MULTIPLE_SCROLL_DPAD,
    .fontId = FONT_NORMAL,
    .cursorKind = CURSOR_BLACK_ARROW
};

static const struct WindowTemplate sWindowTemplate_InviteToActivity = {
    .bg = 0,
    .tilemapLeft = 20,
    .tilemapTop = 5,
    .width = 16,
    .height = 8,
    .paletteNum = 15,
    .baseBlock = 0x0001
};

static const struct ListMenuItem sInviteToActivityMenuItems[] = {
    { sText_Greetings, ACTIVITY_CARD | LINK_GROUP_CAPACITY(0, 2)},
    { sText_Battle,    ACTIVITY_BATTLE_SINGLE | IN_UNION_ROOM | LINK_GROUP_CAPACITY(0, 2)},
    { sText_Chat2,     ACTIVITY_CHAT | IN_UNION_ROOM | LINK_GROUP_CAPACITY(0, 2)},
    { sText_Exit,      ACTIVITY_NONE | IN_UNION_ROOM }
};

static const struct ListMenuTemplate sListMenuTemplate_InviteToActivity = {
    .items = sInviteToActivityMenuItems,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .itemPrintFunc = NULL,
    .totalItems = ARRAY_COUNT(sInviteToActivityMenuItems),
    .maxShowed = 4,
    .windowId = 0,
    .header_X = 0,
    .item_X = 8,
    .cursor_X = 0,
    .upText_Y = 1,
    .cursorPal = 2,
    .fillValue = 1,
    .cursorShadowPal = 3,
    .lettersSpacing = 0,
    .itemVerticalPadding = 0,
    .scrollMultiple = LIST_NO_MULTIPLE_SCROLL,
    .fontId = FONT_NORMAL,
    .cursorKind = CURSOR_BLACK_ARROW
};

static const struct WindowTemplate sWindowTemplate_RegisterForTrade = {
    .bg = 0,
    .tilemapLeft = 18,
    .tilemapTop = 7,
    .width = 16,
    .height = 6,
    .paletteNum = 15,
    .baseBlock = 0x0001
};

static const struct ListMenuItem sRegisterForTradeListMenuItems[] = {
    { gText_Register, 1 },
    { sText_Info, 2 },
    { sText_Exit, 3 }
};

static const struct ListMenuTemplate sListMenuTemplate_RegisterForTrade = {
    .items = sRegisterForTradeListMenuItems,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .itemPrintFunc = NULL,
    .totalItems = ARRAY_COUNT(sRegisterForTradeListMenuItems),
    .maxShowed = 3,
    .windowId = 0,
    .header_X = 0,
    .item_X = 8,
    .cursor_X = 0,
    .upText_Y = 1,
    .cursorPal = 2,
    .fillValue = 1,
    .cursorShadowPal = 3,
    .lettersSpacing = 0,
    .itemVerticalPadding = 0,
    .scrollMultiple = LIST_NO_MULTIPLE_SCROLL,
    .fontId = FONT_NORMAL,
    .cursorKind = CURSOR_BLACK_ARROW
};

static const struct WindowTemplate sWindowTemplate_TradingBoardRequestType = {
    .bg = 0,
    .tilemapLeft = 20,
    .tilemapTop = 1,
    .width = 16,
    .height = 12,
    .paletteNum = 15,
    .baseBlock = 0x0001
};

// Subtract two from the total type count to handle in-battle types not for display
static const struct ListMenuItem sTradingBoardTypes[NUMBER_OF_MON_TYPES - 2] = {
    { gTypesInfo[TYPE_NORMAL].name,   TYPE_NORMAL         },
    { gTypesInfo[TYPE_FIRE].name,     TYPE_FIRE           },
    { gTypesInfo[TYPE_WATER].name,    TYPE_WATER          },
    { gTypesInfo[TYPE_ELECTRIC].name, TYPE_ELECTRIC       },
    { gTypesInfo[TYPE_GRASS].name,    TYPE_GRASS          },
    { gTypesInfo[TYPE_ICE].name,      TYPE_ICE            },
    { gTypesInfo[TYPE_GROUND].name,   TYPE_GROUND         },
    { gTypesInfo[TYPE_ROCK].name,     TYPE_ROCK           },
    { gTypesInfo[TYPE_FLYING].name,   TYPE_FLYING         },
    { gTypesInfo[TYPE_PSYCHIC].name,  TYPE_PSYCHIC        },
    { gTypesInfo[TYPE_FIGHTING].name, TYPE_FIGHTING       },
    { gTypesInfo[TYPE_POISON].name,   TYPE_POISON         },
    { gTypesInfo[TYPE_BUG].name,      TYPE_BUG            },
    { gTypesInfo[TYPE_GHOST].name,    TYPE_GHOST          },
    { gTypesInfo[TYPE_DRAGON].name,   TYPE_DRAGON         },
    { gTypesInfo[TYPE_STEEL].name,    TYPE_STEEL          },
    { gTypesInfo[TYPE_DARK].name,     TYPE_DARK           },
    { gTypesInfo[TYPE_FAIRY].name,    TYPE_FAIRY          },
    { sText_Exit,                NUMBER_OF_MON_TYPES }
};

static const struct ListMenuTemplate sMenuTemplate_TradingBoardRequestType = {
    .items = sTradingBoardTypes,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .itemPrintFunc = NULL,
    .totalItems = ARRAY_COUNT(sTradingBoardTypes),
    .maxShowed = 6,
    .windowId = 0,
    .header_X = 0,
    .item_X = 8,
    .cursor_X = 0,
    .upText_Y = 1,
    .cursorPal = 2,
    .fillValue = 1,
    .cursorShadowPal = 3,
    .lettersSpacing = 0,
    .itemVerticalPadding = 0,
    .scrollMultiple = LIST_NO_MULTIPLE_SCROLL,
    .fontId = FONT_NORMAL,
    .cursorKind = CURSOR_BLACK_ARROW
};

static const struct WindowTemplate sWindowTemplate_TradingBoardHeader = {
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 1,
    .width = 28,
    .height = 2,
    .paletteNum = 13,
    .baseBlock = 0x0001
};

static const struct WindowTemplate sWindowTemplate_TradingBoardMain = {
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 5,
    .width = 28,
    .height = 12,
    .paletteNum = 13,
    .baseBlock = 0x0039
};

static const struct ListMenuItem sTradeBoardListMenuItems[] = {
    { sText_EmptyString, LIST_HEADER },
    { sText_EmptyString,  0 },
    { sText_EmptyString,  1 },
    { sText_EmptyString,  2 },
    { sText_EmptyString,  3 },
    { sText_EmptyString,  4 },
    { sText_EmptyString,  5 },
    { sText_EmptyString,  6 },
    { sText_EmptyString,  7 },
    { sText_Exit2,  8 }
};

static const struct ListMenuTemplate sTradeBoardListMenuTemplate = {
    .items = sTradeBoardListMenuItems,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .itemPrintFunc = TradeBoardListMenuItemPrintFunc,
    .totalItems = ARRAY_COUNT(sTradeBoardListMenuItems),
    .maxShowed = 6,
    .windowId = 0,
    .header_X = 0,
    .item_X = 8,
    .cursor_X = 0,
    .upText_Y = 1,
    .cursorPal = 14,
    .fillValue = 15,
    .cursorShadowPal = 13,
    .lettersSpacing = 0,
    .itemVerticalPadding = 0,
    .scrollMultiple = LIST_NO_MULTIPLE_SCROLL,
    .fontId = FONT_NORMAL,
    .cursorKind = CURSOR_BLACK_ARROW
};

// Unused
static const struct WindowTemplate sWindowTemplate_Unused = {
    .bg = 0,
    .tilemapLeft = 1,
    .tilemapTop = 5,
    .width = 28,
    .height = 12,
    .paletteNum = 13,
    .baseBlock = 0x0039
};

static const struct ListMenuItem sEmptyListMenuItems[] = {
    { sText_EmptyString,  0 },
    { sText_EmptyString,  1 },
    { sText_EmptyString,  2 },
    { sText_EmptyString,  3 },
    { sText_EmptyString,  4 },
    { sText_EmptyString,  5 },
    { sText_EmptyString,  6 },
    { sText_EmptyString,  7 },
    { sText_EmptyString,  8 },
    { sText_EmptyString,  9 },
    { sText_EmptyString, 10 },
    { sText_EmptyString, 11 },
    { sText_EmptyString, 12 },
    { sText_EmptyString, 13 },
    { sText_EmptyString, 14 },
    { sText_EmptyString, 15 }
};

// Unused
static const struct ListMenuTemplate sEmptyListMenuTemplate = {
    .items = sEmptyListMenuItems,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .itemPrintFunc = ItemPrintFunc_EmptyList,
    .totalItems = ARRAY_COUNT(sEmptyListMenuItems),
    .maxShowed = 4,
    .windowId = 0,
    .header_X = 0,
    .item_X = 8,
    .cursor_X = 0,
    .upText_Y = 1,
    .cursorPal = 2,
    .fillValue = 1,
    .cursorShadowPal = 3,
    .lettersSpacing = 0,
    .itemVerticalPadding = 0,
    .scrollMultiple = LIST_MULTIPLE_SCROLL_DPAD,
    .fontId = FONT_NORMAL,
    .cursorKind = CURSOR_BLACK_ARROW
};

static const struct RfuPlayerData sUnionRoomPlayer_DummyRfu = {0};

ALIGNED(4) static const u8 sAcceptedActivityIds_SingleBattle[]    = {ACTIVITY_BATTLE_SINGLE, 0xff};
ALIGNED(4) static const u8 sAcceptedActivityIds_DoubleBattle[]    = {ACTIVITY_BATTLE_DOUBLE, 0xff};
ALIGNED(4) static const u8 sAcceptedActivityIds_MultiBattle[]     = {ACTIVITY_BATTLE_MULTI, 0xff};
ALIGNED(4) static const u8 sAcceptedActivityIds_Trade[]           = {ACTIVITY_TRADE, 0xff};
ALIGNED(4) static const u8 sAcceptedActivityIds_PokemonJump[]     = {ACTIVITY_POKEMON_JUMP, 0xff};
ALIGNED(4) static const u8 sAcceptedActivityIds_BerryCrush[]      = {ACTIVITY_BERRY_CRUSH, 0xff};
ALIGNED(4) static const u8 sAcceptedActivityIds_BerryPicking[]    = {ACTIVITY_BERRY_PICK, 0xff};
ALIGNED(4) static const u8 sAcceptedActivityIds_WonderCard[]      = {ACTIVITY_WONDER_CARD, 0xff};
ALIGNED(4) static const u8 sAcceptedActivityIds_WonderNews[]      = {ACTIVITY_WONDER_NEWS, 0xff};
ALIGNED(4) static const u8 sAcceptedActivityIds_Resume[]          = {
    IN_UNION_ROOM | ACTIVITY_NONE,
    IN_UNION_ROOM | ACTIVITY_BATTLE_SINGLE,
    IN_UNION_ROOM | ACTIVITY_TRADE,
    IN_UNION_ROOM | ACTIVITY_CHAT,
    IN_UNION_ROOM | ACTIVITY_CARD,
    IN_UNION_ROOM | ACTIVITY_ACCEPT,
    IN_UNION_ROOM | ACTIVITY_DECLINE,
    IN_UNION_ROOM | ACTIVITY_NPCTALK,
    IN_UNION_ROOM | ACTIVITY_PLYRTALK,
    0xff
};
ALIGNED(4) static const u8 sAcceptedActivityIds_Init[]            = {ACTIVITY_SEARCH, 0xff};
ALIGNED(4) static const u8 sAcceptedActivityIds_Unk11[]           = {
    ACTIVITY_BATTLE_SINGLE,
    ACTIVITY_BATTLE_DOUBLE,
    ACTIVITY_BATTLE_MULTI,
    ACTIVITY_TRADE,
    ACTIVITY_POKEMON_JUMP,
    ACTIVITY_BERRY_CRUSH,
    ACTIVITY_BERRY_PICK,
    ACTIVITY_WONDER_CARD,
    ACTIVITY_WONDER_NEWS,
    ACTIVITY_SPIN_TRADE,
    0xff
};
ALIGNED(4) static const u8 sAcceptedActivityIds_RecordCorner[]    = {ACTIVITY_RECORD_CORNER, 0xff};
ALIGNED(4) static const u8 sAcceptedActivityIds_BerryBlender[]    = {ACTIVITY_BERRY_BLENDER, 0xff};
ALIGNED(4) static const u8 sAcceptedActivityIds_CoolContest[]     = {ACTIVITY_CONTEST_COOL, 0xff};
ALIGNED(4) static const u8 sAcceptedActivityIds_BeautyContest[]   = {ACTIVITY_CONTEST_BEAUTY, 0xff};
ALIGNED(4) static const u8 sAcceptedActivityIds_CuteContest[]     = {ACTIVITY_CONTEST_CUTE, 0xff};
ALIGNED(4) static const u8 sAcceptedActivityIds_SmartContest[]    = {ACTIVITY_CONTEST_SMART, 0xff};
ALIGNED(4) static const u8 sAcceptedActivityIds_ToughContest[]    = {ACTIVITY_CONTEST_TOUGH, 0xff};
ALIGNED(4) static const u8 sAcceptedActivityIds_BattleTower[]     = {ACTIVITY_BATTLE_TOWER, 0xff};
ALIGNED(4) static const u8 sAcceptedActivityIds_BattleTowerOpen[] = {ACTIVITY_BATTLE_TOWER_OPEN, 0xff};

static const u8 *const sAcceptedActivityIds[NUM_LINK_GROUP_TYPES] = {
    [LINK_GROUP_SINGLE_BATTLE]     = sAcceptedActivityIds_SingleBattle,
    [LINK_GROUP_DOUBLE_BATTLE]     = sAcceptedActivityIds_DoubleBattle,
    [LINK_GROUP_MULTI_BATTLE]      = sAcceptedActivityIds_MultiBattle,
    [LINK_GROUP_TRADE]             = sAcceptedActivityIds_Trade,
    [LINK_GROUP_POKEMON_JUMP]      = sAcceptedActivityIds_PokemonJump,
    [LINK_GROUP_BERRY_CRUSH]       = sAcceptedActivityIds_BerryCrush,
    [LINK_GROUP_BERRY_PICKING]     = sAcceptedActivityIds_BerryPicking,
    [LINK_GROUP_WONDER_CARD]       = sAcceptedActivityIds_WonderCard,
    [LINK_GROUP_WONDER_NEWS]       = sAcceptedActivityIds_WonderNews,
    [LINK_GROUP_UNION_ROOM_RESUME] = sAcceptedActivityIds_Resume,
    [LINK_GROUP_UNION_ROOM_INIT]   = sAcceptedActivityIds_Init,
    [LINK_GROUP_UNK_11]            = sAcceptedActivityIds_Unk11,
    [LINK_GROUP_RECORD_CORNER]     = sAcceptedActivityIds_RecordCorner,
    [LINK_GROUP_BERRY_BLENDER]     = sAcceptedActivityIds_BerryBlender,
    [LINK_GROUP_UNK_14]            = NULL,
    [LINK_GROUP_COOL_CONTEST]      = sAcceptedActivityIds_CoolContest,
    [LINK_GROUP_BEAUTY_CONTEST]    = sAcceptedActivityIds_BeautyContest,
    [LINK_GROUP_CUTE_CONTEST]      = sAcceptedActivityIds_CuteContest,
    [LINK_GROUP_SMART_CONTEST]     = sAcceptedActivityIds_SmartContest,
    [LINK_GROUP_TOUGH_CONTEST]     = sAcceptedActivityIds_ToughContest,
    [LINK_GROUP_BATTLE_TOWER]      = sAcceptedActivityIds_BattleTower,
    [LINK_GROUP_BATTLE_TOWER_OPEN] = sAcceptedActivityIds_BattleTowerOpen
};

static const u8 sLinkGroupToURoomActivity[NUM_LINK_GROUP_TYPES + 2] =
{
    [LINK_GROUP_SINGLE_BATTLE]     = ACTIVITY_BATTLE_SINGLE,
    [LINK_GROUP_DOUBLE_BATTLE]     = ACTIVITY_BATTLE_DOUBLE,
    [LINK_GROUP_MULTI_BATTLE]      = ACTIVITY_BATTLE_MULTI,
    [LINK_GROUP_TRADE]             = ACTIVITY_TRADE,
    [LINK_GROUP_POKEMON_JUMP]      = ACTIVITY_POKEMON_JUMP,
    [LINK_GROUP_BERRY_CRUSH]       = ACTIVITY_BERRY_CRUSH,
    [LINK_GROUP_BERRY_PICKING]     = ACTIVITY_BERRY_PICK,
    [LINK_GROUP_WONDER_CARD]       = ACTIVITY_WONDER_CARD,
    [LINK_GROUP_WONDER_NEWS]       = ACTIVITY_WONDER_NEWS,
    [LINK_GROUP_UNION_ROOM_RESUME] = ACTIVITY_NONE,
    [LINK_GROUP_UNION_ROOM_INIT]   = ACTIVITY_NONE,
    [LINK_GROUP_UNK_11]            = ACTIVITY_NONE,
    [LINK_GROUP_RECORD_CORNER]     = ACTIVITY_RECORD_CORNER,
    [LINK_GROUP_BERRY_BLENDER]     = ACTIVITY_BERRY_BLENDER,
    [LINK_GROUP_UNK_14]            = ACTIVITY_NONE,
    [LINK_GROUP_COOL_CONTEST]      = ACTIVITY_CONTEST_COOL,
    [LINK_GROUP_BEAUTY_CONTEST]    = ACTIVITY_CONTEST_BEAUTY,
    [LINK_GROUP_CUTE_CONTEST]      = ACTIVITY_CONTEST_CUTE,
    [LINK_GROUP_SMART_CONTEST]     = ACTIVITY_CONTEST_SMART,
    [LINK_GROUP_TOUGH_CONTEST]     = ACTIVITY_CONTEST_TOUGH,
    [LINK_GROUP_BATTLE_TOWER]      = ACTIVITY_BATTLE_TOWER,
    [LINK_GROUP_BATTLE_TOWER_OPEN] = ACTIVITY_BATTLE_TOWER_OPEN
};
