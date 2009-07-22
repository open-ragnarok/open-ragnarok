#ifndef __STATES_H
#define __STATES_H

enum ROState {
	ST_Unk = 0,
	ST_ServiceSelect,
	ST_Login,
	ST_CharSelect,
	ST_CharCreate,
	ST_Loading,
	ST_Map
};

#endif /* __STATES_H */
