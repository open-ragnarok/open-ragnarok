/* $Id$ */
#include "stdafx.h"

#include "actgl.h"

ROGL::ActData::ActData() {
	spr = NULL;
	act = NULL;
	m_name = "noname";
}

ROGL::ActData::~ActData() {
	if (spr != NULL)
		delete spr;
	if (act != NULL)
		delete act;
}

void ROGL::ActData::Draw() const {
}

ROGL::ActData::ActData(const ROGL::ActData& _act) {
	*this = _act;
}

ROGL::ActData& ROGL::ActData::operator = (const ROGL::ActData& _act) {
	this->act = _act.act;
	this->spr = _act.spr;
	this->m_name =  _act.m_name;
	return(*this);
}

