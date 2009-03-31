/* $Id$ */
#include "stdafx.h"

#include "ro/constants.h"

namespace RO {

	namespace EUC {
		char user_interface[] =  "\xc0\xaf\xc0\xfa\xc0\xce\xc5\xcd\xc6\xe4\xc0\xcc\xbd\xba";
		char body[] = "\xb8\xf6\xc5\xeb";
		char head[] = "\xb8\xd3\xb8\xae\xc5\xeb";
		char humans[] = "\xc0\xce\xb0\xa3\xc1\xb7"; // Human family

		// === SEX
		char female[] = "\xbf\xa9";
		char male[] = "\xb3\xb2";
		char *sex[] = {
			male, female
		};

		// classes

		char acolyte[] = "";
		char alchemist[] = "";
		char archer[] = "";
		char assassin[] = "";
		char bard[] = "";
		char blacksmith[] = "";
		char crusader[] = "";
		char dancer[] = "";
		char gm[] = "";
		char hunter[] = "";
		char knight[] = "";
		char magician[] = "";
		char mercenary[] = "";
		char merchant[] = "";
		char monk[] = "";
		char novice[] = "\xc3\xca\xba\xb8\xc0\xda";
		char priest[] = "";
		char rogue[] = "";
		char sage[] = "";
		char swordsman[] = "";
		char thief[] = "";
		char wizard[] = "";

		char *classname[] = {
			acolyte,
			alchemist,
			archer,
			assassin,
			bard,
			blacksmith,
			crusader,
			dancer,
			gm,
			hunter,
			knight,
			magician,
			mercenary,
			merchant,
			monk,
			novice,
			priest,
			rogue,
			sage,
			swordsman,
			thief,
			wizard
		};
	}

	namespace UTF {
		char humans[] = "\xec\x9d\xb8\xea\xb0\x84\xec\xa1\xb1"; // Human family
		char monsters[] = "\xeb\xaa\xac\xec\x8a\xa4\xed\x84\xb0";

		char water[] = "\xec\x9b\x8c\xed\x84\xb0";

		// === SEX
		char female[] = "\xec\x97\xac";
		char male[] = "\xeb\x82\xa8";

		char *sex[] = {
			male, female
		};

		// BODY PARTS
		char body[] = "\xeb\xaa\xb8\xed\x86\xb5";
		char head[] = "\xeb\xa8\xb8\xeb\xa6\xac\xed\x86\xb5";
		char weapons[] = "\xeb\xaa\xbd\xed\x81\xac";

		// classes
		char acolyte[] = "\xec\x84\xb1\xec\xa7\x81\xec\x9e";
		char alchemist[] = "\xec\x97\xb0\xea\xb8\x88\xec\x88\xa0\xec\x82\xac";
		char archer[] = "\xea\xb6\x81\xec\x88\x98";
		char assassin[] = "\xec\x96\xb4\xec\x84\xb8\xec\x8b\xa0";
		char bard[] = "\xeb\xb0\x94\xeb\x93\x9c";
		char blacksmith[] = "\xec\xa0\x9c\xec\xb2\xa0";
		char crusader[] = "\xed\x81\xac\xeb\xa3\xa8\xec\x84\xb8\xec\x9d\xb4\xeb\x8d\x94";
		char dancer[] = "\xeb\xac\xb4\xed\x9d\xac";
		char gm[] = "\xec\x94\xb4\xec\x98\x81\xec\x9e\x90";
		char hunter[] = "\xed\x97\x8c\xed\x84\xb0";
		char knight[] = "\xea\xb8\xb0\xec\x82\xac";
		char magician[] = "\xeb\xa7\x88\xeb\xb2\x95\xec\x82\xac";
		char mercenary[] = "";
		char merchant[] = "\xec\x83\x81\xec\x9d\xb8";
		char monk[] = "\xeb\xaa\xbd\xed\x81\xac";
		char novice[] = "\xec\xb4\x88\xeb\xb3\xb4\xec\x9e\x90";
		char priest[] = "\xed\x94\x84\xeb\xa6\xac\xec\x8a\xa4\xed\x8a\xb8";
		char rogue[] = "\xeb\xa1\x9c\xea\xb7\xb8";
		char sage[] = "\xec\x84\xb8\xec\x9d\xb4\xec\xa7\x80";
		char swordsman[] = "\xea\xb2\x80\xec\x82\xac";
		char thief[] = "\xeb\x8f\x84\xeb\x91\x91";
		char wizard[] = "\xec\x9c\x84\xec\xa0\x80\xeb\x93\x9c";

		char *classname[] = {
			acolyte,
			alchemist,
			archer,
			assassin,
			bard,
			blacksmith,
			crusader,
			dancer,
			gm,
			hunter,
			knight,
			magician,
			mercenary,
			merchant,
			monk,
			novice,
			priest,
			rogue,
			sage,
			swordsman,
			thief,
			wizard
		};

		char *classname_en[] = {
			"Acolyte",
			"Alchemist",
			"Archer",
			"Assassin",
			"Bard",
			"Blacksmith",
			"Crusader",
			"Dancer",
			"Hunter",
			"Knight",
			"Magician",
			"Mercenary",
			"Merchant",
			"Monk",
			"Novice",
			"Priest",
			"Rogue",
			"Sage",
			"Swordsman",
			"Thief",
			"Wizard"
		};
	}
}

