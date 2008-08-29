/* $id$ */
#ifndef __CONSTANTS_H
#define __CONSTANTS_H

#include "impdecl.h"

namespace RO {
	enum CSex {
		S_MALE,
		S_FEMALE
	};

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

	namespace UTF {
		MYLIB_DLLAPI extern char humans[]; // Human family
		MYLIB_DLLAPI extern char monsters[];

		MYLIB_DLLAPI extern char water[];

		// sex
		MYLIB_DLLAPI extern char *sex[];
		MYLIB_DLLAPI extern char female[];
		MYLIB_DLLAPI extern char male[];

		MYLIB_DLLAPI extern char body[];
		MYLIB_DLLAPI extern char head[];
		MYLIB_DLLAPI extern char weapons[];

		// classes
		MYLIB_DLLAPI extern char *classname[];
		MYLIB_DLLAPI extern char *classname_en[];

		MYLIB_DLLAPI extern char acolyte[];
		MYLIB_DLLAPI extern char alchemist[];
		MYLIB_DLLAPI extern char archer[];
		MYLIB_DLLAPI extern char assassin[];
		MYLIB_DLLAPI extern char bard[];
		MYLIB_DLLAPI extern char blacksmith[];
		MYLIB_DLLAPI extern char crusader[];
		MYLIB_DLLAPI extern char dancer[];
		MYLIB_DLLAPI extern char hunter[];
		MYLIB_DLLAPI extern char knight[];
		MYLIB_DLLAPI extern char magician[];
		MYLIB_DLLAPI extern char mercenary[];
		MYLIB_DLLAPI extern char merchant[];
		MYLIB_DLLAPI extern char monk[];
		MYLIB_DLLAPI extern char novice[];
		MYLIB_DLLAPI extern char priest[];
		MYLIB_DLLAPI extern char rogue[];
		MYLIB_DLLAPI extern char sage[];
		MYLIB_DLLAPI extern char swordsman[];
		MYLIB_DLLAPI extern char thief[];
		MYLIB_DLLAPI extern char wizard[];
	}
}

#endif /* __CONSTANTS_H */
