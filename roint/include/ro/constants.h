/* $Id$ */
#ifndef __CONSTANTS_H
#define __CONSTANTS_H

#include "roint_import.h"

namespace RO {
	enum CSex {
		S_MALE,
		S_FEMALE
	};

	/** \enum CJob enumeration of available jobs */
	enum CJob {
		J_ACOLYTE = 0,
		J_ALCHEMIST,
		J_ARCHER,
		J_ASSASSIN,
		J_BARD,
		J_BLACKSMITH,
		J_CRUSADER,
		J_DANCER,
		J_GM,
		J_HUNTER,
		J_KNIGHT,
		J_MAGICIAN,
		J_MERCENARY,
		J_MERCHANT,
		J_MONK,
		J_NOVICE,
		J_PRIEST,
		J_ROGUE,
		J_SAGE,
		J_SWORDSMAN,
		J_THIEF,
		J_WIZARD,

		JOB_COUNT
	};

	enum CDir {
		DIR_S,
		DIR_SW,
		DIR_W,
		DIR_NW,
		DIR_N,
		DIR_NE,
		DIR_E,
		DIR_SE
	};

	// How to find sprs?
	// Classes: data\sprite\<humans>\<body>\<sex(male/female)>\<class>_<sex>.(spr/act)
	// Novice: data\sprite\<humans>\<body>\<sex>\<novice>_<sex>...

	namespace EUC {
		ROINT_DLLAPI extern char user_interface[];

		ROINT_DLLAPI extern char body[];
		ROINT_DLLAPI extern char head[];

		ROINT_DLLAPI extern char humans[]; // Human family

		// sex
		ROINT_DLLAPI extern char *sex[];
		ROINT_DLLAPI extern char female[];
		ROINT_DLLAPI extern char male[];

		// classes
		ROINT_DLLAPI extern char *classname[];

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
	}

	namespace UTF {
		ROINT_DLLAPI extern char humans[]; // Human family
		ROINT_DLLAPI extern char monsters[];

		ROINT_DLLAPI extern char water[];

		// sex
		ROINT_DLLAPI extern char *sex[];
		ROINT_DLLAPI extern char female[];
		ROINT_DLLAPI extern char male[];

		ROINT_DLLAPI extern char body[];
		ROINT_DLLAPI extern char head[];
		ROINT_DLLAPI extern char weapons[];

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
	}
}

#endif /* __CONSTANTS_H */
