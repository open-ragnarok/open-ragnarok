#ifndef __RONET_PACKET_IDS_H
#define __RONET_PACKET_IDS_H

namespace ronet {
typedef enum PacketIDs{
	// ============== //
	// == OUTBOUND == //
	// ============== //
	// To LOGIN
	pktLoginID = 0x0064,
	pktLogin2ID = 0x02b0,

	// To CHAR
	pktCharLoginID = 0x0065,		// 17 bytes
	pktCharSelectID = 0x0066,		// 3 bytes -- S 0066 <character number>.B
	pktCharCreateID = 0x0067,		// 37 bytes -- S 0067 <name>.24B <str>.B <agi>.B <vit>.B <int>.B <dex>.B <luk>.B <slot>.B <hair color>.W <hair style>.W
	pktCharDeleteID = 0x0068,		// 46 bytes
	pktCharDelete2ID = 0x01fb,		// 56 bytes
	pktKeepAliveID = 0x0187,

	// To MAP
	pktMapLoginID = 0x0072,			// (19 bytes) S 0072 <account id>.int <char id>.int <session id1>.int <client tick>.unsigned int <gender>.byte
	pktMapMoveID = 0x0085,			// (5 bytes)  S 0085 <x>.10bits <y>.10bits <0>.4bits
	pktGuildInfoRequestID = 0x014d,	// (2 bytes)  S 014d
	pktMapLoadedID = 0x007d,		// (2 bytes)  S 007d
	pktKeepAliveMapID = 0x007e,		// (6 bytes)  S 007e <ticks>.uint
	pktTalkID = 0x0090,				// (7 bytes)  S 0090 <id>.uint <0x01>.byte
	pktNpcContinueID = 0x00b9,		// (6 bytes)  S 00b9 <id>.uint
	pktNpcCancelID = 0x0146,		// (6 bytes)  S 0146 <id>.uint
	pktRequestPlayerInfoID = 0x0094,// (6 bytes)  S 0094 <id>.uint
	pktRequestCharacterNameID = 0x0193,// (6 bytes)  S 0193 <id>.uint
	pktGetStoreInfoID = 0x00c5,		// (7 bytes)  S 00c5 <id>.uint <info>.byte
	pktRequestIgnoreListID = 0x00d3, 

	pktItemPickupID = 0x009f,		// (6 bytes)  S 009f <id>.uint
	pktItemDropID = 0x00a2,			// S 00a2 <index>.w <amount>.w

	pktSendNpcTextID = 0x01d5,		// S 01d5 <size>.short <npcid>.uint <message>.text <0x00>.byte
	pktSendNpcResponseID = 0x00b8,	// (7 bytes)  S 00b8 <npcid>.uint <number>.byte
	pktSendNpcNumberID = 0x0143,	// (10 bytes) S 0143 <npcid>.uint <number>.uint

	pktRestartID = 0x00b2,			// S 00b2 <type>.B
	pktStatusUpID = 0x00bb,			// S 00bb <type>.w <amount>.B
	pktQuitID = 0x018a,				// S 018a <fixed 0>.w
	
	pktReqEmotionID = 0x00bf,		// S 00bf <type>.B

	// Packetver 20
//	pktActionRequestID = 0x0190,	// <packet_id>,<packet_len>,actionrequest,<offset of target_id (4 bytes)>,<offset of action_type (1 byte)> -- Thanks, FlavioJS!
	// Packetver 5
	pktActionRequestID = 0x0089,	// <packet_id>.short <target_id>.uint <action_type>.B

	// Packetver 23
	pktMapLogin23ID = 0x0436,		// (19 bytes) S 0436 <account id>.int <char id>.int <login id>.int <client tick>.unsigned int <gender>.byte
	pktKeepAliveMap23ID = 0x0089,

	// ============== //
	// == INBOUND  == //
	// ============== //
	// From LOGIN
	pktServerListID = 0x0069,
	pktLoginErrorID = 0x006a,		// (23 bytes) S 006A <type>.short 0x00 * 20
	pktAuthFailedID = 0x0081,		// (3 bytes)  S 0081 <type>.short

	// From CHAR
	pktCharListID = 0x006b,
	pktCharSelectionFailID = 0x006c,// S 006c <unk>.short
	pktCharPositionID = 0x0071,		// (30 bytes) S 0071 <charid>.int <mapname>.16B <ip>.int <port>.short
	pktCharCreatedID = 0x006d,		// Also named "NewCharInfo" (108 or 110 bytes)
	pktCharCreateErrorID = 0x006e,	// (3 bytes)
	pktCharRenameID = 0x028d,

	// From MAP
	pktMapLoginSuccessID = 0x0073,	// R 0073 <server tick>.uint <coordidate>.3B ?.2B
	pktOwnSpeechID = 0x008e,		// R 008e <len>.w <str>.?B
	pktMapAcctSendID = 0x0283,		// (6 bytes) R 0283 <account id>.int
	pktSkillListID = 0x010f,		// R 010f <len>.w {<skill ID>.w <target type>.w ?.w <lv>.w <sp>.w <range>.w <skill name>.24B <up>.B}.37B*
	pktUpdateStatusID = 0x00b0,		// R 00b0 <type>.w <val>.l
	pktDisplayStatID = 0x0141,		// R 0141 <type>.l <base>.l <bonus>.l
	pktAttackRangeID = 0x013a,		// R 013a <val>.w
	pktMapMoveOkID = 0x0087,		// (12 bytes) R 0087 <ticks>.int <<start_coord>.20bits <dest_coord>.20bits>.5Bytes <0x88>.B
	pktServerTickID = 0x007f,		// R 007f <server tick>.l

	pktPlayerEquipID = 0x01d7,		// (11 bytes) R 01d7 <id>.int <type>.B <id1>.short <id2>.short
	pktItemGainedID = 0x00a0,		// R 00a0 <index>.short <amount>.short <id>.short <identified>.B <broken>.B <upgrade>.B <cards>.8B <type_equip>.short <type>.B <fail>.B
	pktItemLostID = 0x00af,			// R 00af <index>.w <amount>.w
	pktInventoryItemsID = 0x00a4,	// R 00a4 <size>.short array of (<index>.short <id>.short <type>.B <identified>.B <type_equip>.short <equipped>.short <broken>.B <upgrade>.B <cards>.8B)
	pktInventoryItemsStackableID = 0x01ee, // R 01ee <size>.short array(<index>.short <id>.short <type>.B <unk>.3B <amount>.short <cards>.8B)

//	pktStatInfoID = 0x00b0,			// R 00b0 <type>.short <value>.int
//	pktStatInfo2ID = 0x0141,		// (14 bytes) R 0141 <type>.short 0x0000 <val>.short 0x0000 <val2>.short 0x0000
	pktZenyExpID = 0x00b1,			// R 00b1 <type>.short <value>.int
	pktHotkeyListID = 0x02b9,		// R 02b9 array of 27(struct HotkeyInfo)
	pktStatsInfoID = 0x00bd,		// (44 bytes) R 00bd <status point>.w <STR>.B <STRupP>.B <AGI>.B <AGIupP>.B <VIT>.B <VITupP>.B <INT>.B <INTupP>.B <DEX>.B <DEXupP>.B <LUK>.B <LUKupP>.B <ATK>.w <ATKbonus>.w <MATKmax>.w <MATKmin>.w <DEF>.w <DEFbonus>.w <MDEF>.w <MDEFbonus>.w <HIT>.w <FLEE>.w <FLEEbonus>.w <critical>.w <karma?>.w <manner?>.w
	
	pktGmBroadID = 0x009a,			// R 009a <len>.w <message>.?B
	pktCharLeaveScreenID = 0x0080,	// R 0080 <ID>.l <type>.B
	pktOtherSpeechID = 0x008d,		// R 008d <len>.w <ID>.l <str>.?B
	pktHpUpdatePartyID = 0x0106,	// R 0106 <ID>.l <HP>.w <MaxHP>.w
	
	pktRecvNpcTalkID = 0x00b4,		// R 00b4 <len>.short <id>.int <message>.string
	pktRecvNpcTalkNextID = 0x00b5,	// R 00b5 <len>.short <id>.int
	pktRecvNpcTalkCloseID = 0x00b6,	// R 00b6 <len>.short <id>.int
	pktRecvNpcTalkResponsesID = 0x00b7,
	pktRecvNpcImage2ID = 0x01b3,	// R 01b3 <len>.short <imagename>.string <type>.B <unknown>.unknown <message>.string
	
	pktMapChangeID = 0x0091,		// R 0091 <map>.16B <pos>.int
	pktRecvNpcInputReqID = 0x01d4,	// R 01d4 <id>.int
	pktStatChangedID = 0x013d,		// R 013d <type>.short <amount>.short
	pktRestartCharSelectID = 0x00b3,// R 00b3 <type>.B
	pktStatusUpAckID = 0x00bc,		// R 00bc <type>.w <fail>.B <val>.B
	pktQuitAckID = 0x018b,			// R 018b <fail>.w

	pktEmotionID = 0x00c0,			// R 00c0 <ID>.l <type>.B
	pktNotifyEffect1ID = 0x019b,		// R 019b <ID>.l <type>.l
	// Actor
	pktActorDisplayID = 0x0078,		// R 0078 
	pktActorMoveID = 0x0086,		// R 0086 <id>.uint <<start_coord>.20bits <dest_coord>.20bits>.5Bytes <0x88>.byte <ticks>.uint
	pktActorSpawnID = 0x0079,		// R 0079
	pktActorWalkingID = 0x007b,		// R 007b
	pktActorActionID = 0x008a,		// (29 bytes) R 008a <srcid>.uint <targetid>.uint <ticks>.uint <src_speed>.uint <dst_speed>.uint <damage>.short <div>.short <type>.char <dual_wield_damage>.short
	pktActorStopID = 0x0088,		// (10 bytes) R 0088 <id>.int <x>.short <y>.short
	// GUILD
	pktGuildAlliesEnemiesListID = 0x014c,
	pktGuildMasterMemberID = 0x014e,		// R 014e <id>.int
	pktGuildEmblemID = 0x0152,				// R 0152 <len>.short <guildID>.int <emblemID>.int <emblem>.?
	pktGuildMembersListID = 0x0154,
	pktGuildMemberPositionChangedID = 0x0156,
	pktGuildLeaveID = 0x015a,
	pktGuildExpulsionID = 0x015c,
	pktGuildBrokenID = 0x015e,				// R 0167 <flag>.int
	pktGuildMemberSettingListID = 0x0160,
	pktGuildSkillsListID = 0x0162,
	pktGuildExpulsionListID = 0x0162,
	pktGuildMembersTitleListID = 0x0166,
	pktGuildCreateResultID = 0x0167,		// R 0167 <type>.byte
	pktGuildInviteResultID = 0x0169,		// R 0169 <type>.byte
	pktGuildRequestID = 0x016a,
	pktGuildNameID = 0x016c,
	pktGuildMemberOnlineStatusID = 0x016d,	// R 016d <id>.uint <charid>.uint <online>.uint
	pktGuildNoticeID = 0x016f,
	pktGuildAllyRequestID = 0x0171,
	pktGuildAllianceID = 0x0173,			// R 0173 <flag>.uint
	pktGuildPositionChangedID = 0x0174,
	pktGuildMessageID = 0x017f,				// R 017f <len>.w <message>.?B
	pktGuildOppositionResultID = 0x0181,	// R 0181 <flag>.byte
	pktGuildUnallyID = 0x0184,				// R 0184 <guild_id>.uint <flag>.uint
	pktGuildAllianceAddedID = 0x0185,
	pktGuildInfoID = 0x01b6,
	pktGuildLocationID = 0x1eb,				// R 01eb <id>.uint <x>.short <y>.short
	pktGuildMemberOnlineStatus2ID = 0x01f2,
	// PET
	pktPetCaptureProcessID = 0x19e,	// R 019e
	pktPetCaptureResultID = 0x1a0,	// R 01a0 <type>.byte
	pktPetInfoID = 0x01a2,			// R 01a2 <name> <renameflag> <level> <hungry> <friendly> <accessory> <type>
	pktPetFoodID = 0x01a3,			// R 01a3 <success>.byte <food_id>.short
	pktPetInfo2ID = 0x01a4,			// R 01a4 <type>.byte <id>.int <value>.int
	pktPetEmotionID = 0x01aa,		// R 01aa <id>.int <type>.int
	pktEggListID = 0x1a6,			// R 01a6

	// == "WTF!?"s == //
	pktUnknown1 = 0x2974, // 74 29 00 04 05 00 d0
} pktIds;

}

#endif /* __RONET_PACKET_IDS_H */
