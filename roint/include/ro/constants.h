/* $Id$ */
#ifndef __CONSTANTS_H
#define __CONSTANTS_H

#include "roint_import.h"

namespace RO {
	enum CSex {
		S_MALE,
		S_FEMALE
	};

	/** \enum CJob enumeration of available jobs with their respective ID [kR105] */
	// Taken from Aegis
	enum CJob {
		J_NOVICE,
		J_SWORDMAN,
		J_MAGE,
		J_ARCHER,
		J_ACOLYTE,
		J_MERCHANT,
		J_THIEF,
		J_KNIHTG,
		J_PRIEST,
		J_WIZARD,
		J_BLACKSMITH,
		J_HUNTER,
		J_ASSASSIN,
		J_KNIGHT2, //Mounted in peco
		J_CRUSADER,
		J_MONK,
		J_SAGE,
		J_ROGUE,
		J_ALCHEMIST,
		J_BARD,
		J_DANCER,
		J_CRUSADER2, //Mounted in peco
	};

	/** \enum CSkill enumeration of available skills with their respective ID [kR105] */
	// Taken from Aegis & http://opensvn.csie.org/EnglishTranslation/English/skillnametable.txt
	enum CSkill {
		NV_BASIC = 1,

		SM_SWORD,
		SM_TWOHAND,
		SM_RECOVERY,
		SM_BASH,
		SM_PROVOKE,
		SM_MAGNUM,
		SM_ENDURE,

		MG_SRECOVERY,
		MG_SIGHT,
		MG_NAPALMBEAT,
		MG_SAFETYWALL,
		MG_SOULSTRIKE,
		MG_COLDBOLT,
		MG_FROSTDIVER,
		MG_STONECURSE,
		MG_FIREBALL,
		MG_FIREWALL,
		MG_FIREBOLT,
		MG_LIGHTNINGBOLT,
		MG_THUNDERSTORM,

		AL_DP,
		AL_DEMONBANE,
		AL_RUWACH,
		AL_PNEUMA,
		AL_TELEPORT,
		AL_WARP,
		AL_HEAL,
		AL_INCAGI,
		AL_DECAGI,
		AL_HOLYWATER,
		AL_CRUCIS,
		AL_ANGELUS,
		AL_BLESSING,
		AL_CURE,

		MC_INCCARRY,
		MC_DISCOUNT,
		MC_OVERCHARGE,
		MC_PUSHCART,
		MC_IDENTIFY,
		MC_VENDING,
		MC_MAMMONITE,

		AC_OWL,
		AC_VULTURE,
		AC_CONCENTRATION,
		AC_DOUBLE,
		AC_SHOWER,

		TF_DOUBLE,
		TF_MISS,
		TF_STEAL,
		TF_HIDING,
		TF_POISON,
		TF_DETOXIFY,

		ALL_RESURRECTION,

		KN_SPEARMASTERY,
		KN_PIERCE,
		KN_BRANDISHSPEAR,
		KN_SPEARSTAB,
		KN_SPEARBOOMERANG,
		KN_TWOHANDQUICKEN,
		KN_AUTOCOUNTER,
		KN_BOWLINGBASH,
		KN_RIDING,
		KN_CAVALIERMASTERY,

		PR_MACEMASTERY,
		PR_IMPOSITIO,
		PR_SUFFRAGIUM,
		PR_ASPERSIO,
		PR_BENEDICTIO,
		PR_SANCTUARY,
		PR_SLOWPOISON,
		PR_STRECOVERY,
		PR_KYRIE,
		PR_MAGNIFICAT,
		PR_GLORIA,
		PR_LEXDIVINA,
		PR_TURNUNDEAD,
		PR_LEXAETERNA,
		PR_MAGNUS,

		WZ_FIREPILLAR,
		WZ_SIGHTRASHER,
		WZ_FIREIVY,
		WZ_METEOR,
		WZ_JUPITEL,
		WZ_VERMILION,
		WZ_WATERBALL,
		WZ_ICEWALL,
		WZ_FROSTNOVA,
		WZ_STORMGUST,
		WZ_EARTHSPIKE,
		WZ_HEAVENDRIVE,
		WZ_QUAGMIRE,
		WZ_ESTIMATION,

		BS_IRON,
		BS_STEEL,
		BS_ENCHANTEDSTONE,
		BS_ORIDEOCON,
		BS_DAGGER,
		BS_SWORD,
		BS_TWOHANDSWORD,
		BS_AXE,
		BS_MACE,
		BS_KNUCKLE,
		BS_SPEAR,
		BS_HILTBINDING,
		BS_FINDINGORE,
		BS_WEAPONRESEARCH,
		BS_REPAIRWEAPON,
		BS_SKINTEMPER,
		BS_HAMMERFALL,
		BS_ADRENALINE,
		BS_WEAPONPERFECT,
		BS_OVERTHRUST,
		BS_MAXIMIZE,

		HT_SKIDTRAP,
		HT_LANDMINE,
		HT_ANKLESNARE,
		HT_SHOCKWAVE,
		HT_SANDMAN,
		HT_FLASHER,
		HT_FREEZINGTRAP,
		HT_BLASTMINE,
		HT_CLAYMORETRAP,
		HT_REMOVETRAP,
		HT_TALKIEBOX,
		HT_BEASTBANE,
		HT_FALCON,
		HT_STEELCROW,
		HT_BLITZBEAT,
		HT_DETECTING,
		HT_SPRINGTRAP,

		AS_RIGHT,
		AS_LEFT,
		AS_KATAR,
		AS_CLOAKING,
		AS_SONICBLOW,
		AS_GRIMTOOTH,
		AS_ENCHANTPOISON,
		AS_POISONREACT,
		AS_VENOMDUST,
		AS_SPLASHER,

		//Platinum Skills
		NV_FIRSTAID,
		NV_TRICKDEAD,
		SM_MOVINGRECOVERY,
		SM_FATALBLOW,
		SM_AUTOBERSERK,
		AC_MAKINGARROW,
		AC_CHARGEARROW,
		TF_SPRINKLESAND,
		TF_BACKSLIDING,
		TF_PICKSTONE,
		TF_THROWSTONE,
		MC_CARTREVOLUTION,
		MC_CHANGECART,
		MC_LOUD,
		AL_HOLYLIGHT,
		MG_ENERGYCOAT
	};

	/** \enum CDir enumeration of cardinal points*/
	//Taken from eAthena
	enum CDir {
		DIR_N,
		DIR_NW,
		DIR_W,
		DIR_SW,
		DIR_S,
		DIR_SE,
		DIR_E,
		DIR_NE,
		DIR_N2 //Yes, North2.. for some reason the original client uses 0 and 9 to reffer to North :S
	};

	// How to find sprs?
	// Classes: data\sprite\<humans>\<body>\<sex(male/female)>\<class>_<sex>.(spr/act)
	// Novice: data\sprite\<humans>\<body>\<sex>\<novice>_<sex>...

	namespace EUC {
		ROINT_DLLAPI extern char user_interface[];

		ROINT_DLLAPI extern char body[];
		ROINT_DLLAPI extern char head[];
		ROINT_DLLAPI extern char weapons[];

		ROINT_DLLAPI extern char humans[]; // Human family
		ROINT_DLLAPI extern char monsters[];

		ROINT_DLLAPI extern char water[];

		// sex
		ROINT_DLLAPI extern char *sex[];
		ROINT_DLLAPI extern char female[];
		ROINT_DLLAPI extern char male[];

		// classes
		ROINT_DLLAPI extern char *classname[];
		ROINT_DLLAPI extern char *classname_en[];

		ROINT_DLLAPI extern char acolyte[];
		ROINT_DLLAPI extern char alchemist[];
		ROINT_DLLAPI extern char archer[];
		ROINT_DLLAPI extern char assassin[];
		ROINT_DLLAPI extern char bard[];
		ROINT_DLLAPI extern char blacksmith[];
		ROINT_DLLAPI extern char crusader[];
		ROINT_DLLAPI extern char dancer[];
		ROINT_DLLAPI extern char hunter[];
		ROINT_DLLAPI extern char knight[];
		ROINT_DLLAPI extern char magician[];
		ROINT_DLLAPI extern char mercenary[];
		ROINT_DLLAPI extern char merchant[];
		ROINT_DLLAPI extern char monk[];
		ROINT_DLLAPI extern char novice[];
		ROINT_DLLAPI extern char priest[];
		ROINT_DLLAPI extern char rogue[];
		ROINT_DLLAPI extern char sage[];
		ROINT_DLLAPI extern char swordsman[];
		ROINT_DLLAPI extern char thief[];
		ROINT_DLLAPI extern char wizard[];
		ROINT_DLLAPI extern char gm[];
		ROINT_DLLAPI extern char crusader_mounted[];
		ROINT_DLLAPI extern char knight_mounted[];
	}
}

#endif /* __CONSTANTS_H */
