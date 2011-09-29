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

const char* user_interface =  "\xc0\xaf\xc0\xfa\xc0\xce\xc5\xcd\xc6\xe4\xc0\xcc\xbd\xba";

// BODY PARTS
const char* body = "\xb8\xf6\xc5\xeb";
const char* head = "\xb8\xd3\xb8\xae\xc5\xeb";
const char* head2 = "\xb8\xd3\xb8\xae"; // Used by palette
const char* weapons = "\xb8\xf9\xc5\xa9";

//CHARACTER TYPE
const char* humans = "\xc0\xce\xb0\xa3\xc1\xb7"; // Human family
const char* monsters = "\xb8\xf3\xbd\xba\xc5\xcd";

//
const char* shields = "\xb9\xe6\xc6\xd0";
const char* items = "\xbe\xc6\xc0\xcc\xc5\xdb";
const char* accessories = "\xbe\xc7\xbc\xbc\xbb\xe7\xb8\xae"; // Head accessories (Helm)
const char* effects = "\xc0\xcc\xc6\xd1\xc6\xae";

const char* water = "\xbf\xf6\xc5\xcd";

// === SEX
const char* female = "\xbf\xa9";
const char* male = "\xb3\xb2";
const char* sex[] = {
	male, female
};

// Weapons
const char* sword = "\xb0\xcb";
const char* dagger = "\xb4\xdc\xb0\xcb";
const char* axe = "\xb5\xb5\xb3\xa2";
const char* staff = "\xb7\xd4\xb5\xe5";
const char* mace = "\xc5\xac\xb7\xb4";
const char* spear = "\xc3\xa2\x0d\x0a";
const char* claw = "\xb3\xca\xc5\xac";
const char* book = "\xc3\xa5";
const char* katar = "\xc4\xab\xc5\xb8\xb8\xa3";
const char* bow = "\xc8\xb0";
const char* instrument = "\xbe\xc7\xb1\xe2";
const char* whip = "\xc3\xa4\xc2\xef";

const char* afterimage = "\xb0\xcb\xb1\xa4"; // Weapons's after image

//const char* arrow = "\x\x\x\x";

// classes [kR105]
const char* novice = "\xC3\xCA\xBA\xB8\xC0\xDA";
const char* swordsman = "\xB0\xCB\xBB\xE7";
const char* magician = "\xB8\xB6\xB9\xFD\xBB\xE7";
const char* archer = "\xB1\xC3\xBC\xF6";
const char* acolyte = "\xBC\xBA\xC1\xF7\x9F\xE0";
const char* merchant = "\xBB\xF3\xC0\xCE";
const char* thief = "\xB5\xB5\xB5\xCF";
const char* knight = "\xB1\xE2\xBB\xE7";
const char* priest = "\xC7\xC1\xB8\xAE\xBD\xBA\xC6\xAE";
const char* wizard = "\xC0\xA7\xC0\xFA\xB5\xE5";
const char* blacksmith = "\xC1\xA6\xC3\xB6";
const char* hunter = "\xC7\xE5\xC5\xCD";
const char* assassin = "\xBE\xEE\xBC\xBC\xBD\xC5";
const char* knight_mounted = "\xC6\xE4\xC4\xDA\xC6\xE4\xC4\xDA\x5F\xB1\xE2\xBB\xE7";
const char* crusader = "\xC5\xA9\xB7\xE7\xBC\xBC\xC0\xCC\xB4\xF5";
const char* monk = "\xB8\xF9\xC5\xA9";
const char* sage = "\xBC\xBC\xC0\xCC\xC1\xF6";
const char* rogue = "\xB7\xCE\xB1\xD7";
const char* alchemist = "\xBF\xAC\xB1\xDD\xBC\xFA\xBB\xE7";
const char* bard = "\xB9\xD9\xB5\xE5";
const char* dancer = "\xB9\xAB\xC8\xF1";
const char* crusader_mounted = "\xBD\xC5\xC6\xE4\xC4\xDA\xC5\xA9\xB7\xE7\xBC\xBC\xC0\xCC\xB4\xF5";
const char* gm = "\x9D\xC4\xBF\xB5\xC0\xDA";
const char* mercenary = "\xBF\xEB\xBA\xB4";

#define CLASSNAME_COUNT 24

const char* classname[] = {
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

const char* classname_en[] = {
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


const char* dnames[] = {
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
