ALIGNED(4) static const u8 sText_EmptyString[] = _("");
ALIGNED(4) static const u8 sText_Colon[] = _(":");
ALIGNED(4) static const u8 sText_ID[] = _("{ID}");
ALIGNED(4) static const u8 sText_PleaseStartOver[] = _("{JPN}もういちど さいしょから\nてつづきを やりなおして ください");
ALIGNED(4) static const u8 sText_WirelessSearchCanceled[] = _("ワイヤレスつうしんの\nけんさくを やめました。");
ALIGNED(4) static const u8 sText_AwaitingCommunucation2[] = _("{JPN}ともだちからの れんらくを\nまっています"); // Unused
ALIGNED(4) static const u8 sText_AwaitingCommunication[] = _("{JPN}{STR_VAR_1}!\nともだちからの れんらくを まっています");
ALIGNED(4) static const u8 sText_AwaitingLinkPressStart[] = _("{JPN}{STR_VAR_1}! れんらくまち!\nにんずうが そろったら STARTボタン");
ALIGNED(4) static const u8 sJPText_SingleBattle[] = _("{JPN}シングルバトルを かいさいする");
ALIGNED(4) static const u8 sJPText_DoubleBattle[] = _("{JPN}ダブルバトルを かいさいする");
ALIGNED(4) static const u8 sJPText_MultiBattle[] = _("{JPN}マルチバトルを かいさいする");
ALIGNED(4) static const u8 sJPText_TradePokemon[] = _("{JPN}ポケモンこうかんを かいさいする");
ALIGNED(4) static const u8 sJPText_Chat[] = _("{JPN}チャットを かいさいする");
ALIGNED(4) static const u8 sJPText_DistWonderCard[] = _("{JPN}ふしぎなカードをくばる");
ALIGNED(4) static const u8 sJPText_DistWonderNews[] = _("{JPN}ふしぎなニュースをくばる");
ALIGNED(4) static const u8 sJPText_DistMysteryEvent[] = _("{JPN}ふしぎなできごとを かいさいする"); // Unused
ALIGNED(4) static const u8 sJPText_HoldPokemonJump[] = _("{JPN}なわとびを かいさいする");
ALIGNED(4) static const u8 sJPText_HoldBerryCrush[] = _("{JPN}きのみマッシャーを かいさいする");
ALIGNED(4) static const u8 sJPText_HoldBerryPicking[] = _("{JPN}きのみどりを かいさいする");
ALIGNED(4) static const u8 sJPText_HoldSpinTrade[] = _("{JPN}ぐるぐるこうかんを かいさいする");
ALIGNED(4) static const u8 sJPText_HoldSpinShop[] = _("{JPN}ぐるぐるショップを かいさいする");

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

ALIGNED(4) static const u8 sText_BButtonCancel[] = _("{JPN}{B_BUTTON}やめる");
ALIGNED(4) static const u8 sJPText_SearchingForParticipants[] = _("ため\nさんかしゃ ぼしゅうちゅう です？"); // Unused, may have been cut off
ALIGNED(4) static const u8 sText_PlayerContactedYouForXAccept[] = _("{JPN}{STR_VAR_2}から れんらくあり!\n{STR_VAR_1}を しますか?");
ALIGNED(4) static const u8 sText_PlayerContactedYouShareX[] = _("{JPN}{STR_VAR_2}から れんらくあり!\n{STR_VAR_1}を くばりますか?");
ALIGNED(4) static const u8 sText_PlayerContactedYouAddToMembers[] = _("{JPN}{STR_VAR_2}から れんらくあり!\nメンバーに しますか?");
ALIGNED(4) static const u8 sText_AreTheseMembersOK[] = _("{JPN}{STR_VAR_1}!\nこの メンバーで よろしいですか?");
ALIGNED(4) static const u8 sText_CancelModeWithTheseMembers[] = _("{JPN}この メンバーで {STR_VAR_1}を\nするのは やめますか?");
ALIGNED(4) static const u8 sText_AnOKWasSentToPlayer[] = _("{JPN}{STR_VAR_1}に OKの\nへんじを だしました");
ALIGNED(4) static const u8 sText_OtherTrainerUnavailableNow[] = _("{JPN}あいての\nつごうが わるいみたい…\p");
ALIGNED(4) static const u8 sText_CantTransmitTrainerTooFar[] = _("{JPN}まだ とおくの かたとは\nつうしんは できません\p");
ALIGNED(4) static const u8 sText_TrainersNotReadyYet[] = _("{JPN}あいての じゅんびが\nできていません\p");

static const u8 *const sCantTransmitToTrainerTexts[] = {
    [UR_TRADE_PLAYER_NOT_READY - 1]  = sText_CantTransmitTrainerTooFar,
    [UR_TRADE_PARTNER_NOT_READY - 1] = sText_TrainersNotReadyYet
};

ALIGNED(4) static const u8 sText_ModeWithTheseMembersWillBeCanceled[] = _("{JPN}この メンバーで {STR_VAR_1}を\nするのは やめます{PAUSE 60}");
ALIGNED(4) static const u8 sText_MemberNoLongerAvailable[] = _("{JPN}つごうが わるくなってしまった\nメンバーが います\p");

static const u8 *const sPlayerUnavailableTexts[] = {
    sText_OtherTrainerUnavailableNow,
    sText_MemberNoLongerAvailable
};

ALIGNED(4) static const u8 sText_TrainerAppearsUnavailable[] = _("あいての トレーナーは\nつうしんできないようです...\p");
ALIGNED(4) static const u8 sText_PlayerSentBackOK[] =    _("{JPN}{STR_VAR_1}から OKとの\nへんじが ありました!");
ALIGNED(4) static const u8 sText_PlayerOKdRegistration[] = _("{JPN}{STR_VAR_1}から メンバー とうろく\nOKとの れんらくが きました!");
ALIGNED(4) static const u8 sText_PlayerRepliedNo[] = _("{JPN}{STR_VAR_1}から NGとの\nへんじが きてしまいました…\p");
ALIGNED(4) static const u8 sText_AwaitingOtherMembers[] = _("{JPN}{STR_VAR_1}!\nほかの メンバーを まっています!");
ALIGNED(4) static const u8 sText_QuitBeingMember[] = _("{JPN}メンバーから ぬけますか?");
ALIGNED(4) static const u8 sText_StoppedBeingMember[] = _("{JPN}メンバーから ぬけました\p");

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
ALIGNED(4) static const u8 sText_LinkWithFriendDropped[] = _("{JPN}おともだちとの つうしんが\nせつだん されました…");
ALIGNED(4) static const u8 sText_PlayerRepliedNo2[] = _("{JPN}{STR_VAR_1}から NGとの\nへんじが きてしまいました…");

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
ALIGNED(4) static const u8 sText_AwaitingPlayersResponseAboutTrade[] = _("{JPN}{STR_VAR_1}からの こうかんの\nへんじを まっています…");
ALIGNED(4) static const u8 sText_Communicating[] = _("{JPN}はなしかけています{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・\n{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・");
ALIGNED(4) static const u8 sText_CommunicatingWithPlayer[] = _("{JPN}{STR_VAR_1}に はなしかけています{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・\n{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・");
ALIGNED(4) static const u8 sText_PleaseWaitAWhile[] = _("{JPN}しょうしょうおまちください{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・\n{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・{PAUSE 15}・");

static const u8 *const sCommunicatingWaitTexts[] = {
    sText_Communicating,
    sText_CommunicatingWithPlayer,
    sText_PleaseWaitAWhile
};

ALIGNED(4) static const u8 sText_HiDoSomethingMale[] = _("{JPN}やあ!\nなにか するかい?");
ALIGNED(4) static const u8 sText_HiDoSomethingFemale[] = _("{JPN}こんにちは!\nなにか する?");
ALIGNED(4) static const u8 sText_HiDoSomethingAgainMale[] = _("{JPN}{STR_VAR_1}「やあ また あったね!\nこんどは なにをする?");
ALIGNED(4) static const u8 sText_HiDoSomethingAgainFemale[] = _("{JPN}{STR_VAR_1}「あ! {PLAYER}\nこんにちは! また なにかする?");

static const u8 *const sHiDoSomethingTexts[][GENDER_COUNT] = {
    {
        sText_HiDoSomethingMale,
        sText_HiDoSomethingFemale
    }, {
        sText_HiDoSomethingAgainMale,
        sText_HiDoSomethingAgainFemale
    }
};

ALIGNED(4) static const u8 sText_DoSomethingMale[] = _("{JPN}なにか するかい?");
ALIGNED(4) static const u8 sText_DoSomethingFemale[] = _("{JPN}なにか する?");
ALIGNED(4) static const u8 sText_DoSomethingAgainMale[] = _("{JPN}{STR_VAR_1}「こんどは なにをする?");
ALIGNED(4) static const u8 sText_DoSomethingAgainFemale[] = _("{JPN}{STR_VAR_1}「また なにかする?"); // Unused

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

ALIGNED(4) static const u8 sText_SomebodyHasContactedYou[] = _("{JPN}だれかが\nはなしかけて きました{PAUSE 60}");
ALIGNED(4) static const u8 sText_PlayerHasContactedYou[] = _("{JPN}{STR_VAR_1}が\nはなしかけて きました{PAUSE 60}");

static const u8 *const sPlayerContactedYouTexts[] = {
    sText_SomebodyHasContactedYou,
    sText_PlayerHasContactedYou
};

ALIGNED(4) static const u8 sText_AwaitingResponseFromTrainer[] = _("{JPN}あいての へんじを\nまっています");
ALIGNED(4) static const u8 sText_AwaitingResponseFromPlayer[] = _("{JPN}{STR_VAR_1}の へんじを\nまっています");

static const u8 *const sAwaitingResponseTexts[] = {
    sText_AwaitingResponseFromTrainer,
    sText_AwaitingResponseFromPlayer
};

ALIGNED(4) static const u8 sText_ShowTrainerCard[] = _("{JPN}あいてが トレーナーカードを\nみせて きました\pこちらも トレーナーカードを\nみせますか?");
ALIGNED(4) static const u8 sText_BattleChallenge[] = _("{JPN}あいてが たいせんを\nもうしこんで きました\pたいせん しますか?");
ALIGNED(4) static const u8 sText_ChatInvitation[] = _("{JPN}あいてが チャット に\nさそって きました\pいっしょに チャット\nしますか?");
ALIGNED(4) static const u8 sText_OfferToTradeMon[] = _("{JPN}とうろく していた\nLV{DYNAMIC 0}の {DYNAMIC 1} と\pLV{DYNAMIC 2}の {DYNAMIC 3} の\nこうかん もうしこみが きています\pこうかん しますか?");
ALIGNED(4) static const u8 sText_OfferToTradeEgg[] = _("{JPN}とうろく していた タマゴの\nこうかん もうしこみが きています\lこうかん しますか?");
ALIGNED(4) static const u8 sText_ChatDropped[] = _("{JPN}かいわが きれました\p");
ALIGNED(4) static const u8 sText_OfferDeclined1[] = _("{JPN}もうしこみを\nことわり ました\p");
ALIGNED(4) static const u8 sText_OfferDeclined2[] = _("{JPN}おさそいを\nことわり ました\p");
ALIGNED(4) static const u8 sText_ChatEnded[] = _("{JPN}かいわを きりました\p");

// Unused
static const u8 *const sInvitationTexts[] = {
    sText_ShowTrainerCard,
    sText_BattleChallenge,
    sText_ChatInvitation,
    sText_OfferToTradeMon
};

ALIGNED(4) static const u8 sText_JoinChatMale[] = _("{JPN}お! いま おれたち\nチャットちゅう なんだ!\lよかったら いっしょに はいるかい?");
ALIGNED(4) static const u8 sText_PlayerJoinChatMale[] = _("{JPN}{STR_VAR_1}「お! {PLAYER}\nいま チャットちゅう なんだ!\lよかったら いっしょに はいるかい?");
ALIGNED(4) static const u8 sText_JoinChatFemale[] = _("{JPN}あ! いまね わたしたち\nチャット してるんだ\lよかったら いっしょに しない?");
ALIGNED(4) static const u8 sText_PlayerJoinChatFemale[] = _("{JPN}{STR_VAR_1}「あ! {PLAYER}\nいまね チャット してるんだ\lよかったら いっしょに しない?");

static const u8 *const sJoinChatTexts[][GENDER_COUNT] = {
    {
        sText_JoinChatMale,
        sText_JoinChatFemale
    }, {
        sText_PlayerJoinChatMale,
        sText_PlayerJoinChatFemale
    }
};

ALIGNED(4) static const u8 sText_TrainerAppearsBusy[] = _("{JPN}……\nいまは とりこみちゅうの ようだ\p");
ALIGNED(4) static const u8 sText_WaitForBattleMale[] = _("{JPN}たいせんだね!\nよーし ちょっと まってて!");
ALIGNED(4) static const u8 sText_WaitForChatMale[] = _("{JPN}チャットだね!\nわかった ちょっと まってて!");
ALIGNED(4) static const u8 sText_ShowTrainerCardMale[] = _("{JPN}わかった! じゃあ あいさつがわりに\nトレーナーカードを みせてあげるよ");
ALIGNED(4) static const u8 sText_WaitForBattleFemale[] = _("{JPN}たいせんね!\nよーし ちょっと まってて!");
ALIGNED(4) static const u8 sText_WaitForChatFemale[] = _("{JPN}チャットね!\nわかったわ ちょっと まってて!");
ALIGNED(4) static const u8 sText_ShowTrainerCardFemale[] = _("{JPN}じゃあ あいさつの かわりに\nトレーナーカードを みせてあげるね");

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

ALIGNED(4) static const u8 sText_WaitForChatMale2[] = _("{JPN}チャットだね!\nわかった ちょっと まってて!"); // Unused
ALIGNED(4) static const u8 sText_DoneWaitingBattleMale[] = _("{JPN}またせたね!\nそれじゃあ たいせん しようか!{PAUSE 60}");
ALIGNED(4) static const u8 sText_DoneWaitingChatMale[] = _("{JPN}よし! じゃあ いっしょに\nチャット しようぜ!{PAUSE 60}");
ALIGNED(4) static const u8 sText_DoneWaitingBattleFemale[] = _("{JPN}おまたせ!\nそれじゃあ はじめましょう!{PAUSE 60}");
ALIGNED(4) static const u8 sText_DoneWaitingChatFemale[] = _("{JPN}おまたせ! じゃあ いっしょに\nチャット しましょう!{PAUSE 60}");
ALIGNED(4) static const u8 sText_TradeWillBeStarted[] = _("{JPN}こうかんを\nかいし します{PAUSE 60}");
ALIGNED(4) static const u8 sText_BattleWillBeStarted[] = _("{JPN}たいせんを\nかいし します!{PAUSE 60}");
ALIGNED(4) static const u8 sText_EnteringChat[] = _("{JPN}チャットを\nかいし します!{PAUSE 60}");

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

ALIGNED(4) static const u8 sText_BattleDeclinedMale[] = _("{JPN}ごめん! ちょっと ポケモンの\nちょうしが わるい みたいなんだ\lまた こんど たいせん しよう\p");
ALIGNED(4) static const u8 sText_BattleDeclinedFemale[] = _("{JPN}ごめんなさい!\nちょっと ポケモンの ちょうしが\lわるい みたいなの……\pまた こんど\nたいせん しようね!\p");

static const u8 *const sBattleDeclinedTexts[GENDER_COUNT] = {
    sText_BattleDeclinedMale,
    sText_BattleDeclinedFemale
};

ALIGNED(4) static const u8 sText_ShowTrainerCardDeclinedMale[] = _("{JPN}あれ!? トレーナーカードが\nみあたらない なあ\lごめん! またこんどね\p");
ALIGNED(4) static const u8 sText_ShowTrainerCardDeclinedFemale[] = _("{JPN}あれ!? トレーナーカード\nどこ いったかな?……\lごめん! またこんどね\p");

static const u8 *const sShowTrainerCardDeclinedTexts[GENDER_COUNT] = {
    sText_ShowTrainerCardDeclinedMale,
    sText_ShowTrainerCardDeclinedFemale
};

ALIGNED(4) static const u8 sText_IfYouWantToDoSomethingMale[] = _("{JPN}なにか したくなったら\nまた こえを かけてくれよ\p");
ALIGNED(4) static const u8 sText_IfYouWantToDoSomethingFemale[] = _("{JPN}なにか したくなったら\nまた こえを かけてね!\p");

static const u8 *const sIfYouWantToDoSomethingTexts[GENDER_COUNT] = {
    sText_IfYouWantToDoSomethingMale,
    sText_IfYouWantToDoSomethingFemale
};

ALIGNED(4) static const u8 sText_TrainerBattleBusy[] = _("{JPN}あ! ごめん\nほかに ようじが あるから\lまたこんどね\p");
ALIGNED(4) static const u8 sText_NeedTwoMonsOfLevel30OrLower1[] = _("{JPN}たいせんを するためには\nレベル30より ひくい ポケモンが\l2ひき ひつよう だよ\p");
ALIGNED(4) static const u8 sText_NeedTwoMonsOfLevel30OrLower2[] = _("{JPN}たいせんを するためには\nレベル30 よりひくい ポケモンが\l2ひき ひつようです\p");

ALIGNED(4) static const u8 sText_DeclineChatMale[] = _("{JPN}そうか\nまた いつでも おいでよ!\p");
ALIGNED(4) static const u8 stext_DeclineChatFemale[] = _("{JPN}そう……\nまた いつでも きてね!\p");

// Response from partner when player declines chat
static const u8 *const sDeclineChatTexts[GENDER_COUNT] = {
    sText_DeclineChatMale,
    stext_DeclineChatFemale
};

ALIGNED(4) static const u8 sText_ChatDeclinedMale[] = _("{JPN}あっ ごめん!\nやっぱり いま つごうがわるいんだ\lまた こんど チャット しような\p");
ALIGNED(4) static const u8 sText_ChatDeclinedFemale[] = _("{JPN}ごめん!\nやっぱり いま つごうが わるいの\lまた こんど チャット しようね\p");

// Response from partner when they decline chat
static const u8 *const sChatDeclinedTexts[GENDER_COUNT] = {
    sText_ChatDeclinedMale,
    sText_ChatDeclinedFemale
};

ALIGNED(4) static const u8 sText_YoureToughMale[] = _("{JPN}おー!\nなかなか てごわいな!\p");
ALIGNED(4) static const u8 sText_UsedGoodMoveMale[] = _("{JPN}ここで そんな わざを\nだして くるなんて やるな!\p");
ALIGNED(4) static const u8 sText_BattleSurpriseMale[] = _("{JPN}さすがだ!\nそんな たたかいかたも あるのか!\p");
ALIGNED(4) static const u8 sText_SwitchedMonsMale[] = _("{JPN}この ポケモンを\nこんな ばめんで だしてくるとは!\p");
ALIGNED(4) static const u8 sText_YoureToughFemale[] = _("{JPN}その ポケモン\nすごくよく そだてられてるね!\p");
ALIGNED(4) static const u8 sText_UsedGoodMoveFemale[] = _("{JPN}そうよ!\nそこで このわざよ!!\p");
ALIGNED(4) static const u8 sText_BattleSurpriseFemale[] = _("{JPN}すごい!\nそんな たたかいかたも あるのね!\p");
ALIGNED(4) static const u8 sText_SwitchedMonsFemale[] = _("{JPN}ポケモン いれかえの タイミングが\nぜつみょうね!\p");

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

ALIGNED(4) static const u8 sText_LearnedSomethingMale[] = _("{JPN}なるほど!\nへー ふむふむ\p");
ALIGNED(4) static const u8 sText_ThatsFunnyMale[] = _("{JPN}また へんなこと いって\nあんまり わらわせるなよ!\p");
ALIGNED(4) static const u8 sText_RandomChatMale1[] = _("{JPN}へー\nそんなことが あったんだ\p");
ALIGNED(4) static const u8 sText_RandomChatMale2[] = _("{JPN}……ふんふん なに?\nと いうことは こういうことか?\p");
ALIGNED(4) static const u8 sText_LearnedSomethingFemale[] = _("{JPN}……え?\nそんなこと しらなかったわ!\p");
ALIGNED(4) static const u8 sText_ThatsFunnyFemale[] = _("{JPN}あはは!\nなにそれ!\p");
ALIGNED(4) static const u8 sText_RandomChatFemale1[] = _("{JPN}あっ そうそう!\nそのことよ!\p");
ALIGNED(4) static const u8 sText_RandomChatFemale2[] = _("{JPN}つまりね……\nうん! それそれ!\p");

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

ALIGNED(4) static const u8 sText_ShowedTrainerCardMale1[] = _("{JPN}いま あいさつがわりに\nトレーナーカードを みせてるんだ\p");
ALIGNED(4) static const u8 sText_ShowedTrainerCardMale2[] = _("{JPN}これからも\nよろしく!\p");
ALIGNED(4) static const u8 sText_ShowedTrainerCardFemale1[] = _("{JPN}いま あいさつの かわりに\nトレーナーカードを みせあってるの\p");
ALIGNED(4) static const u8 sText_ShowedTrainerCardFemale2[] = _("{JPN}これからも\nよろしくね!\p");

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

ALIGNED(4) static const u8 sText_MaleTraded1[] = _("{JPN}やった!\nこのポケモン ほしかったんだ!\p");
ALIGNED(4) static const u8 sText_MaleTraded2[] = _("{JPN}さがしてた ポケモンを\nやっと こうかんして もらったよ\p\n");
ALIGNED(4) static const u8 sText_FemaleTraded1[] = _("{JPN}いまね ポケモンの\nこうかんを してるの!\p");
ALIGNED(4) static const u8 sText_FemaleTraded2[] = _("{JPN}ほしい ポケモンが\nやっと こうかんして もらえたわ!\p");

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

ALIGNED(4) static const u8 sText_XCheckedTradingBoard[] = _("{JPN}{STR_VAR_1}は\nこうかんけいじばんを みた!\p");
ALIGNED(4) static const u8 sText_RegisterMonAtTradingBoard[] = _("{JPN}いらっしゃいませ\nここでは こうかんけいじばん に\pごじぶんの ポケモンを とうろく\nすることが できます\pとうろく しますか?");
ALIGNED(4) static const u8 sText_TradingBoardInfo[] = _("{JPN}この こうかんけいじばん に\nポケモンを とうろく しておくと\pけいじばんを みた トレーナーが\nその とうろくされた ポケモンと\pてもちの ポケモンを\nこうかん することが できます\pぜひポケモンを とうろくして\nいろいろな トレーナー との\lこうかんを たのしんで ください\pとうろく しますか?");
ALIGNED(4) static const u8 sText_ThankYouForRegistering[] = _("{JPN}こうかんけいじばん の とうろくが\nかんりょう しました\pごりよう ありがとう\nございました!\p"); // unused
ALIGNED(4) static const u8 sText_NobodyHasRegistered[] = _("{JPN}けいじばんに だれも ポケモンを\nとうろく していません\p\n"); // unused
ALIGNED(4) static const u8 sText_ChooseRequestedMonType[] = _("{JPN}ほしい ポケモンの タイプを\nえらんで ください\n");
ALIGNED(4) static const u8 sText_WhichMonWillYouOffer[] = _("{JPN}こうかんに だす ポケモンを\nてもちから えらんで ください\p");
ALIGNED(4) static const u8 sText_RegistrationCanceled[] = _("{JPN}とうろくを ちゅうし しました\p");
ALIGNED(4) static const u8 sText_RegistrationCompleted[] = _("{JPN}とうろくが かんりょう しました!\p");
ALIGNED(4) static const u8 sText_TradeCanceled[] = _("{JPN}こうかんを キャンセルしました\p");
ALIGNED(4) static const u8 sText_CancelRegistrationOfMon[] = _("{JPN}とうろく されている {STR_VAR_1}\nレベル{STR_VAR_2}を かいじょ しますか?");
ALIGNED(4) static const u8 sText_CancelRegistrationOfEgg[] = _("{JPN}とうろく されている タマゴを\nかいじょ しますか?");
ALIGNED(4) static const u8 sText_RegistrationCanceled2[] = _("{JPN}とうろくを かいじょ しました\p");
ALIGNED(4) static const u8 sText_TradeTrainersWillBeListed[] = _("{JPN}こうかんを きぼうしているひとを\nひょうじします"); // unused
ALIGNED(4) static const u8 sText_ChooseTrainerToTradeWith2[] = _("{JPN}こうかん したい トレーナーを\nえらんで ください"); // unused
ALIGNED(4) static const u8 sText_AskTrainerToMakeTrade[] = _("{JPN}{STR_VAR_1}に こうかんを\nもうしこみ ますか?");
ALIGNED(4) static const u8 sText_AwaitingResponseFromTrainer2[] = _("{JPN}……\nあいての へんじを まっています"); // unused
ALIGNED(4) static const u8 sText_NotRegisteredAMonForTrade[] = _("{JPN}あなたが こうかんにだす\nポケモンが とうろくされていません\p"); // unused
ALIGNED(4) static const u8 sText_DontHaveTypeTrainerWants[] = _("{JPN}てもちに\n{STR_VAR_1}の ほしがっている\l{STR_VAR_2}ポケモンが ありません\p");
ALIGNED(4) static const u8 sText_DontHaveEggTrainerWants[] = _("{JPN}{STR_VAR_1}の ほしがっている\nタマゴを もっていません\p");
ALIGNED(4) static const u8 sText_PlayerCantTradeForYourMon[] = _("{JPN}いまは {STR_VAR_1}は あなたの\nポケモンと こうかんできません\p");
ALIGNED(4) static const u8 sText_CantTradeForPartnersMon[] = _("{JPN}{STR_VAR_1}の ポケモンとは\nいまは こうかんできません\p");

// Unused
static const u8 *const sCantTradeMonTexts[] = {
    sText_PlayerCantTradeForYourMon,
    sText_CantTradeForPartnersMon
};

ALIGNED(4) static const u8 sText_TradeOfferRejected[] = _("{JPN}こうかんを\nことわられ ました\p");
ALIGNED(4) static const u8 sText_EggTrade[] = _("{JPN}タマゴのこうかん");
ALIGNED(4) static const u8 sText_ChooseJoinCancel[] = _("{DPAD_UPDOWN}えらぶ  {A_BUTTON}さんか  {B_BUTTON}やめる");
ALIGNED(4) static const u8 sText_ChooseTrainer[] = _("{JPN}ともだちを えらんでください");
ALIGNED(4) static const u8 sText_ChooseTrainerSingleBattle[] = _("{JPN}シングルバトルをする ともだちを\nえらんでください");
ALIGNED(4) static const u8 sText_ChooseTrainerDoubleBattle[] = _("{JPN}ダブルバトルをする ともだちを\nえらんでください");
ALIGNED(4) static const u8 sText_ChooseLeaderMultiBattle[] = _("{JPN}マルチバトル!\nリーダーを えらんでください");
ALIGNED(4) static const u8 sText_ChooseTrainerToTradeWith[] = _("{JPN}ポケモンの こうかんをする\nともだちを えらんでください");
ALIGNED(4) static const u8 sText_ChooseTrainerToShareWonderCards[] = _("{JPN}ふしぎなカードを くばっている\nともだちを えらんでください");
ALIGNED(4) static const u8 sText_ChooseTrainerToShareWonderNews[] = _("{JPN}ふしぎなニュースを くばっている\nともだちを えらんでください");
ALIGNED(4) static const u8 sText_ChooseLeaderPokemonJump[] = _("{JPN}ミニポケモンでジャンプ!\nリーダーを えらんでください");
ALIGNED(4) static const u8 sText_ChooseLeaderBerryCrush[] = _("{JPN}きのみクラッシュ!\nリーダーを えらんでください");
ALIGNED(4) static const u8 sText_ChooseLeaderBerryPicking[] = _("{JPN}ドードリオのきのみどり!\nリーダーを えらんでください");
ALIGNED(4) static const u8 sText_ChooseLeaderBerryBlender[] = _("{JPN}きのみブレンダー!\nリーダーを えらんでください");
ALIGNED(4) static const u8 sText_ChooseLeaderRecordCorner[] = _("{JPN}レコードコーナー!\nリーダーを えらんでください");
ALIGNED(4) static const u8 sText_ChooseLeaderCoolContest[] = _("{JPN}かっこよさコンテスト!\nリーダーを えらんでください");
ALIGNED(4) static const u8 sText_ChooseLeaderBeautyContest[] = _("{JPN}うつくしさコンテスト!\nリーダーを えらんでください");
ALIGNED(4) static const u8 sText_ChooseLeaderCuteContest[] = _("{JPN}かわいさコンテスト!\nリーダーを えらんでください");
ALIGNED(4) static const u8 sText_ChooseLeaderSmartContest[] = _("{JPN}かしこさコンテスト!\nリーダーを えらんでください");
ALIGNED(4) static const u8 sText_ChooseLeaderToughContest[] = _("{JPN}たくましさコンテスト!\nリーダーを えらんでください");
ALIGNED(4) static const u8 sText_ChooseLeaderBattleTowerLv50[] = _("{JPN}バトルタワー レベル50!\nリーダーを えらんでください");
ALIGNED(4) static const u8 sText_ChooseLeaderBattleTowerOpenLv[] = _("{JPN}バトルタワー オープンレベル!\nリーダーを えらんでください");

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
