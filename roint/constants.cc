/* $Id$ */
#include "stdafx.h"

#include "constants.h"

namespace RO {
	namespace UTF {
		char humans[] = "\xec\x9d\xb8\xea\xb0\x84\xec\xa1\xb1"; // Human family
		char monsters[] = "\xeb\xaa\xac\xec\x8a\xa4\xed\x84\xb0";

		char water[] = "\xec\x9b\x8c\xed\x84\xb0";
		// char humans[] = { 0xec, 0x9d, 0xb8, 0xea, 0xb0, 0x84, 0xec, 0xa1, 0xb1, 0 }; // Human family
		// char monsters[] = { 0xeb, 0xaa, 0xac, 0xec, 0x8a, 0xa4, 0xed, 0x84, 0xb0 , 0};

		// char water[] = { 0xec, 0x9b, 0x8c, 0xed, 0x84, 0xb0, 0 };

		// === SEX
		char female[] = "\xec\x97\xac";
		char male[] = "\xeb\x82\xa8";

		//char female[] = { 0xec, 0x97, 0xac, 0 };
		//char male[] = { 0xeb, 0x82, 0xa8, 0 };
		char *sex[] = {
				male, female
		};

		// BODY PARTS
		char body[] = "\xeb\xaa\xb8\xed\x86\xb5";
		char head[] = "\xeb\xa8\xb8\xeb\xa6\xac\xed\x86\xb5";
		char weapons[] = "\xeb\xaa\xbd\xed\x81\xac";
		//char body[] = { 0xeb, 0xaa, 0xb8, 0xed, 0x86, 0xb5, 0 };
		//char head[] = { 0xeb, 0xa8, 0xb8, 0xeb, 0xa6, 0xac, 0xed, 0x86, 0xb5, 0 };
		//char weapons[] = { 0xeb, 0xaa, 0xbd, 0xed, 0x81, 0xac, 0};

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

		/*
		char acolyte[] = { 0xec, 0x84, 0xb1, 0xec, 0xa7, 0x81, 0xec, 0x9e, 0 };
		char alchemist[] = { 0xec, 0x97, 0xb0, 0xea, 0xb8, 0x88, 0xec, 0x88, 0xa0, 0xec, 0x82, 0xac, 0 };
		char archer[] = { 0xea, 0xb6, 0x81, 0xec, 0x88, 0x98, 0};
		char assassin[] = { 0xec, 0x96, 0xb4, 0xec, 0x84, 0xb8, 0xec, 0x8b, 0xa0, 0 };
		char bard[] = { 0xeb, 0xb0, 0x94, 0xeb, 0x93, 0x9c, 0 };
		char blacksmith[] = { 0xec, 0xa0, 0x9c, 0xec, 0xb2, 0xa0, 0 };
		char crusader[] = { 0xed, 0x81, 0xac, 0xeb, 0xa3, 0xa8, 0xec, 0x84, 0xb8, 0xec, 0x9d, 0xb4, 0xeb, 0x8d, 0x94, 0 };
		char dancer[] = { 0xeb, 0xac, 0xb4, 0xed, 0x9d, 0xac, 0 };
		char gm[] = { 0xec, 0x94, 0xb4, 0xec, 0x98, 0x81, 0xec, 0x9e, 0x90, 0 };
		char hunter[] = { 0xed, 0x97, 0x8c, 0xed, 0x84, 0xb0, 0};
		char knight[] = { 0xea, 0xb8, 0xb0, 0xec, 0x82, 0xac, 0 };
		char magician[] = { 0xeb, 0xa7, 0x88, 0xeb, 0xb2, 0x95, 0xec, 0x82, 0xac,0};
		char mercenary[] = { 0 };
		char merchant[] = { 0xec, 0x83, 0x81, 0xec, 0x9d, 0xb8, 0 };
		char monk[] = { 0xeb, 0xaa, 0xbd, 0xed, 0x81, 0xac, 0 };
		char novice[] = { 0xec, 0xb4, 0x88, 0xeb, 0xb3, 0xb4, 0xec, 0x9e, 0x90, 0 };
		char priest[] = { 0xed, 0x94, 0x84, 0xeb, 0xa6, 0xac, 0xec, 0x8a, 0xa4, 0xed, 0x8a, 0xb8, 0 };
		char rogue[] = { 0xeb, 0xa1, 0x9c, 0xea, 0xb7, 0xb8, 0 };
		char sage[] = { 0xec, 0x84, 0xb8, 0xec, 0x9d, 0xb4, 0xec, 0xa7, 0x80, 0 };
		char swordsman[] = { 0xea, 0xb2, 0x80, 0xec, 0x82, 0xac, 0 };
		char thief[] = { 0xeb, 0x8f, 0x84, 0xeb, 0x91, 0x91, 0 };
		char wizard[] = { 0xec, 0x9c, 0x84, 0xec, 0xa0, 0x80, 0xeb, 0x93, 0x9c, 0 };
		*/

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

