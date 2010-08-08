/* $Id$ */
/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of The Open Ragnarok Project
    Copyright 2007 - 2009 The Open Ragnarok Team
    For the latest information visit http://www.open-ragnarok.org
    ------------------------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place - Suite 330, Boston, MA 02111-1307, USA, or go to
    http://www.gnu.org/copyleft/lesser.txt.
    ------------------------------------------------------------------------------------
*/
#include "stdafx.h"

#include "ro/constants.h"

namespace ro {

namespace EUC {

char user_interface[] =  "\xc0\xaf\xc0\xfa\xc0\xce\xc5\xcd\xc6\xe4\xc0\xcc\xbd\xba";

// BODY PARTS
char body[] = "\xb8\xf6\xc5\xeb";
char head[] = "\xb8\xd3\xb8\xae\xc5\xeb";
char head2[] = "\xb8\xd3\xb8\xae"; // Used by palette
char weapons[] = "\xb8\xf9\xc5\xa9";

//CHARACTER TYPE
char humans[] = "\xc0\xce\xb0\xa3\xc1\xb7"; // Human family
char monsters[] = "\xb8\xf3\xbd\xba\xc5\xcd";

//
char shields[] = "\xb9\xe6\xc6\xd0";
char items[] = "\xbe\xc6\xc0\xcc\xc5\xdb";
char accesories[] = "\xbe\xc7\xbc\xbc\xbb\xe7\xb8\xae";
char effects[] = "\xc0\xcc\xc6\xd1\xc6\xae";

char water[] = "\xbf\xf6\xc5\xcd";

// === SEX
char female[] = "\xbf\xa9";
char male[] = "\xb3\xb2";
char *sex[] = {
	male, female
};

// classes [kR105]
char novice[] = "\xC3\xCA\xBA\xB8\xC0\xDA";
char swordsman[] = "\xB0\xCB\xBB\xE7";
char magician[] = "\xB8\xB6\xB9\xFD\xBB\xE7";
char archer[] = "\xB1\xC3\xBC\xF6";
char acolyte[] = "\xBC\xBA\xC1\xF7\x9F\xE0";
char merchant[] = "\xBB\xF3\xC0\xCE";
char thief[] = "\xB5\xB5\xB5\xCF";
char knight[] = "\xB1\xE2\xBB\xE7";
char priest[] = "\xC7\xC1\xB8\xAE\xBD\xBA\xC6\xAE";
char wizard[] = "\xC0\xA7\xC0\xFA\xB5\xE5";
char blacksmith[] = "\xC1\xA6\xC3\xB6";
char hunter[] = "\xC7\xE5\xC5\xCD";
char assassin[] = "\xBE\xEE\xBC\xBC\xBD\xC5";
char knight_mounted[] = "";
char crusader[] = "\xC5\xA9\xB7\xE7\xBC\xBC\xC0\xCC\xB4\xF5";
char monk[] = "\xB8\xF9\xC5\xA9";
char sage[] = "\xBC\xBC\xC0\xCC\xC1\xF6";
char rogue[] = "\xB7\xCE\xB1\xD7";
char alchemist[] = "\xBF\xAC\xB1\xDD\xBC\xFA\xBB\xE7";
char bard[] = "\xB9\xD9\xB5\xE5";
char dancer[] = "\xB9\xAB\xC8\xF1";
char crusader_mounted[] = "";
char gm[] = "\x9D\xC4\xBF\xB5\xC0\xDA";
char mercenary[] = "";

#define CLASSNAME_COUNT 24

char *classname[] = {
	novice,
	swordsman,
	magician,
	archer,
	acolyte,
	merchant,
	thief,
	knight,
	priest,
	wizard,
	blacksmith,
	hunter,
	assassin,
	knight_mounted,
	crusader,
	monk,
	sage,
	rogue,
	alchemist,
	bard,
	dancer,
	crusader_mounted,
	gm,
	mercenary,
	NULL
};

const char* getClassName(unsigned int id) {
	static char unkname[64];
	if (id >= CLASSNAME_COUNT) {
		sprintf(unkname, "UknownClass%d", id);
		return (unkname);
	}

	return(classname[id]);
}

} /* namespace EUC */

char *classname_en[] = {
	"Novice",
	"Swordsman",
	"Magician",
	"Archer",
	"Acolyte",
	"Merchant",
	"Thief",
	"Knight",
	"Priest",
	"Wizard",
	"Blacksmith",
	"Hunter",
	"Assassin",
	"Knight", // Mounted
	"Crusader",
	"Monk",
	"Sage",
	"Rogue",
	"Alchemist",
	"Bard",
	"Dancer",
	"Crusader", // Mounted
	"GM",
	"Mercenary",
	NULL
};

const char* getClassNameEN(unsigned int id) {
	static char unkname[64];
	if (id >= CLASSNAME_COUNT) {
		sprintf(unkname, "UknownClass%d", id);
		return (unkname);
	}

	return(classname_en[id]);
}


char *dnames[] = {
	"Speed",
	"Base Exp",
	"Job Exp",
	"Karma",
	"Manner",
	"HP",
	"Max HP",
	"SP",
	"Max SP",
	"Status Point",
	"0a",
	"Base Level",
	"Skill Point",
	"STR",
	"AGI",
	"VIT",
	"INT",
	"DEX",
	"LUK",
	"Class",
	"Zeny",
	"Sex",
	"Next base exp",
	"Next job exp",
	"Weight",
	"Max Weight",
	"1a",
	"1b",
	"1c",
	"1d",
	"1e",
	"1f",
	"Up STR",
	"Up AGI",
	"Up VIT",
	"Up INT",
	"Up DEX",
	"Up LUK",
	"26",
	"27",
	"28",
	"ATK1",
	"ATK2",
	"MATK1",
	"MATK2",
	"DEF1",
	"DEF2",
	"MDEF1",
	"MDEF2",
	"HIT",
	"FLEE1",
	"FLEE2",
	"Critical",
	"ASPD",
	"36",
	"Job Level",
	"Upper",
	"Partner",
	"Cart",
	"Fame",
	"Unbreakable"
};

} /* namespace ro */
