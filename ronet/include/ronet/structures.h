#ifndef __OPENRO_RO_STRUCTURES_H
#define __OPENRO_RO_STRUCTURES_H

struct CharAttributes {
	unsigned char Str;
	unsigned char Agi;
	unsigned char Vit;
	unsigned char Int;
	unsigned char Dex;
	unsigned char Luk;
};

struct CharInformation {
	unsigned int id;
	unsigned int base_xp;
	int zeny;
	unsigned int job_xp;
	unsigned int job_lv;
	// unsigned int opt1;
	// unsigned int opt2;
	unsigned int option;
	unsigned int karma;
	unsigned int manner;
	unsigned short status_point;
	unsigned short hp;
	unsigned short max_hp;
	unsigned short sp;
	unsigned short max_sp;
	unsigned short speed;
	unsigned short Class;
	unsigned short hair;
	unsigned short weapon;
	unsigned short base_lv;
	unsigned short skill_point;
	unsigned short head_bottom;
	unsigned short shield;
	unsigned short head_top;
	unsigned short head_mid;
	unsigned short head_color;
	unsigned short clothes_color;
	char name[24];
	CharAttributes attributes;
	unsigned char slot;
	unsigned short rename;
};

#endif /* __OPENRO_RO_STRUCTURES_H */