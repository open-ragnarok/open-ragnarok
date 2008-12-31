/* $Id$ */
#include "stdafx.h"

#include "basic_objects.h"

const std::string EmptyObject::getObjectType() const { return("Empty"); }
const std::string EmptyObject::toString() const { return("<>"); }

TextObject::TextObject() {}
TextObject::TextObject(const std::string& s) { data = s; }
TextObject::~TextObject() {}
const std::string TextObject::getObjectType() const { return("Text"); }
const std::string TextObject::toString() const { return(data); }
TextObject TextObject::operator = (const std::string& s) { data = s; return(*this); }
TextObject TextObject::operator = (const TextObject& o) { data = o.data; return(*this); }

IntObject::IntObject() : BasicObject<t_stor>() {}
IntObject::IntObject(const t_stor& d) : BasicObject<t_stor>(d) {}
IntObject::IntObject(const IntObject& o) : BasicObject<t_stor>(o) {}
IntObject::IntObject(const IntObject* o) : BasicObject<t_stor>(o) {}
IntObject::IntObject(const std::string& s) { data = atoi(s.c_str()); }
IntObject::~IntObject() {}

const std::string IntObject::getObjectType() const {
	return("Integer");
}

const std::string IntObject::toString() const {
	std::string ret;
	char buf[256];
	sprintf(buf, "%d", data);
	ret = buf;
	return(ret);
}

FloatObject::FloatObject() : BasicObject<t_stor>() {}
FloatObject::FloatObject(const t_stor& d) : BasicObject<t_stor>(d) {}
FloatObject::FloatObject(const FloatObject& o) : BasicObject<t_stor>(o) {}
FloatObject::FloatObject(const FloatObject* o) : BasicObject<t_stor>(o) {}
FloatObject::FloatObject(const std::string& s) { data = (t_stor)atof(s.c_str()); }
FloatObject::~FloatObject() {}

const std::string FloatObject::getObjectType() const {
	return("Float");
}

const std::string FloatObject::toString() const {
	std::string ret;
	char buf[256];
	sprintf(buf, "%f", data);
	ret = buf;
	return(ret);
}