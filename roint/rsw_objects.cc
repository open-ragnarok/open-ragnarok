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
#include "ro/types/rsw.h"
#include "ro/ro.h"

namespace ro {

// ===== OBJECT
RSW::Object::Object(RSW::ObjectType t) : m_type(t) {
}

RSW::Object::~Object() {
}

RSW::Object* RSW::Object::readStream(std::istream& s, const s_obj_ver& ver) {
	int objtype;
	s.read((char*)&objtype, sizeof(int));
	if (s.fail())
		return(NULL);

	switch ((ObjectType)objtype) {
		case ModelType:
			{
				ModelObject* obj = new ModelObject();
				if (!obj->readData(s, ver)) {
					delete obj;
					return(NULL);
				}
				return obj;
			}
			break;
		case LightType:
			{
				LightObject* obj = new LightObject();
				if (!obj->readData(s, ver)) {
					delete obj;
					return(NULL);
				}
				return obj;
			}
			break;
		case SoundType:
			{
				SoundObject* obj = new SoundObject();
				if (!obj->readData(s, ver)) {
					delete obj;
					return(NULL);
				}
				return obj;
			}
			break;
		case EffectType:
			{
				EffectObject* obj = new EffectObject();
				if (!obj->readData(s, ver)) {
					delete obj;
					return(NULL);
				}
				return obj;
			}
			break;
		default:
			fprintf(stderr, "ERROR! invalid object %d\n", objtype);
			return(NULL);
	}
}

bool RSW::Object::writeStream(std::ostream& s, const s_obj_ver& ver) const {
	s.write((char*)&m_type, sizeof(int));
	if (s.fail())
		return(false);

	bool ret = false;
	switch (m_type) {
		case ModelType:
			ret = ((const ModelObject*)this)->writeData(s, ver);
			break;
		case LightType:
			ret = ((const LightObject*)this)->writeData(s, ver);
			break;
		case SoundType:
			ret = ((const SoundObject*)this)->writeData(s, ver);
			break;
		case EffectType:
			ret = ((const EffectObject*)this)->writeData(s, ver);
			break;
	}
	return(ret);
}

RSW::ObjectType RSW::Object::getType() const {
	return(m_type);
}

bool RSW::Object::isType(RSW::ObjectType t) const {
	return(m_type == t);
}

RSW::Object* RSW::Object::Copy() const {
	Object* ret = NULL;
	switch(m_type) {
		case ModelType:
			ret = new ModelObject(*((const ModelObject*)this));
			break;
		case LightType:
			ret = new LightObject(*((const LightObject*)this));
			break;
		case SoundType:
			ret = new SoundObject(*((const SoundObject*)this));
			break;
		case EffectType:
			ret = new EffectObject(*((const EffectObject*)this));
			break;
	}
	return(ret);
}


// ===== MODEL
RSW::ModelObject::ModelObject() : Object(RSW::ModelType) {
	memset(name, 0, sizeof(name));
	animType = 0;
	animSpeed = 1.0f;
	blockType = 0;
	memset(modelName, 0, sizeof(modelName));
	memset(nodeName, 0, sizeof(nodeName));
	pos[0] = pos[1] = pos[2] = 0.0f;
	rot[0] = rot[1] = rot[2] = 0.0f;
	scale[0] = scale[1] = scale[2] = 0.0f;
}

RSW::ModelObject::ModelObject(const ModelObject& obj) : Object(RSW::ModelType) {
	*this = obj;
}

RSW::ModelObject::~ModelObject() {
}

bool RSW::ModelObject::readData(std::istream& s, const s_obj_ver& ver) {
	if ((ver.cver.major == 1 && ver.cver.minor >= 3) || ver.cver.major > 1) {
		s.read(name, 40);
		name[39] = 0;
		s.read((char*)&animType, sizeof(int));
		s.read((char*)&animSpeed, sizeof(float));
		if (animSpeed < 0.0f || animSpeed >= 100.0f)
			animSpeed = 1.0f;// not [0 100[
		s.read((char*)&blockType, sizeof(int));
	}
	else {
		name[0] = 0;
		animType = 0;
		animSpeed = 1.0f;
		blockType = 0;
	}
	s.read(modelName, 80);
	s.read(nodeName, 80);
	s.read((char*)&pos, sizeof(float) * 3);
	s.read((char*)&rot, sizeof(float) * 3);
	s.read((char*)&scale, sizeof(float) * 3);
	return(!s.fail());
}

bool RSW::ModelObject::writeData(std::ostream& s, const s_obj_ver& ver) const {
	if ((ver.cver.major == 1 && ver.cver.minor >= 3) || ver.cver.major > 1) {
		s.write(name, 40);
		s.write((char*)&animType, sizeof(int));
		s.write((char*)&animSpeed, sizeof(float));
		s.write((char*)&blockType, sizeof(int));
	}
	s.write(modelName, 80);
	s.write(nodeName, 80);
	s.write((char*)&pos, sizeof(float) * 3);
	s.write((char*)&rot, sizeof(float) * 3);
	s.write((char*)&scale, sizeof(float) * 3);
	return(!s.fail());
}

void RSW::ModelObject::Dump(std::ostream& o, const std::string& pfx) const {
	o << pfx << "ModelObject" << std::endl;
	o << pfx << "\tName: " << name << std::endl;
	o << pfx << "\tAnim type: " << animType << std::endl;
	o << pfx << "\tAnim speed: " << animSpeed << std::endl;
	o << pfx << "\tBlock type: " << blockType << std::endl;
	o << pfx << "\tModel name: " << modelName << std::endl;
	o << pfx << "\tNode name: " << nodeName << std::endl;
	o << pfx << "\tPos: " << pos[0] << ", " << pos[1] << ", " << pos[2] << std::endl;
	o << pfx << "\tRot: " << rot[0] << ", " << rot[1] << ", " << rot[2] << std::endl;
	o << pfx << "\tScale: " << scale[0] << ", " << scale[1] << ", " << scale[2] << std::endl;
}

RSW::ModelObject& RSW::ModelObject::operator = (const ModelObject& obj) {
	memcpy(name, obj.name, sizeof(name));
	animType = obj.animType;
	animSpeed = obj.animSpeed;
	blockType = obj.blockType;
	memcpy(modelName, obj.modelName, sizeof(modelName));
	memcpy(nodeName, obj.nodeName, sizeof(nodeName));
	memcpy(pos, obj.pos, sizeof(pos));
	memcpy(rot, obj.rot, sizeof(rot));
	memcpy(scale, obj.scale, sizeof(scale));
	return(*this);
}


// ===== LIGHT
RSW::LightObject::LightObject() : Object(RSW::LightType) {
}

RSW::LightObject::LightObject(const LightObject& obj) : Object(RSW::LightType) {
	*this = obj;
}

RSW::LightObject::~LightObject() {
}

bool RSW::LightObject::readData(std::istream& s, const s_obj_ver& ver) {
	s.read((char*)&name, 80);
	name[79] = 0;
	s.read((char*)&pos, sizeof(float) * 3);
	s.read((char*)&red, sizeof(int));
	s.read((char*)&green, sizeof(int));
	s.read((char*)&blue, sizeof(int));
	s.read((char*)&range, sizeof(float));
	return(!s.fail());
}

bool RSW::LightObject::writeData(std::ostream& s, const s_obj_ver& ver) const {
	s.write((char*)&name, 80);
	s.write((char*)&pos, sizeof(float) * 3);
	s.write((char*)&red, sizeof(int));
	s.write((char*)&green, sizeof(int));
	s.write((char*)&blue, sizeof(int));
	s.write((char*)&range, sizeof(float));
	return(!s.fail());
}

void RSW::LightObject::Dump(std::ostream& o, const std::string& pfx) const {
	o << pfx << "LightObject" << std::endl;
	o << pfx << "\tName: " << name << std::endl;
	o << pfx << "\tPos: " << pos[0] << ", " << pos[1] << ", " << pos[2] << std::endl;
	o << pfx << "\tRed: " << red << std::endl;
	o << pfx << "\tGreen: " << green << std::endl;
	o << pfx << "\tBlue: " << blue << std::endl;
	o << pfx << "\tRange: " << range << std::endl;
}

RSW::LightObject& RSW::LightObject::operator = (const LightObject& obj) {
	memcpy(name, obj.name, sizeof(name));
	memcpy(pos, obj.pos, sizeof(pos));
	red = obj.red;
	green = obj.green;
	blue = obj.blue;
	range = obj.range;
	return(*this);
}


// ===== SOUND
RSW::SoundObject::SoundObject() : Object(RSW::SoundType) {
}

RSW::SoundObject::SoundObject(const SoundObject& obj) : Object(RSW::SoundType) {
	*this = obj;
}

RSW::SoundObject::~SoundObject() {
}

bool RSW::SoundObject::readData(std::istream& s, const s_obj_ver& ver) {
	s.read(name, 80);
	s.read(waveName, 80);
	name[79] = waveName[79] = 0;
	s.read((char*)&pos, sizeof(float) * 3);
	s.read((char*)&vol, sizeof(float));
	s.read((char*)&width, sizeof(int));
	s.read((char*)&height, sizeof(int));
	s.read((char*)&range, sizeof(float));
	if (ver.cver.major >= 2)
		s.read((char*)&cycle, sizeof(float));
	else
		cycle = 4.0f;
	return(!s.fail());
}

bool RSW::SoundObject::writeData(std::ostream& s, const s_obj_ver& ver) const {
	s.write(name, 80);
	s.write(waveName, 80);
	s.write((char*)&pos, sizeof(float) * 3);
	s.write((char*)&vol, sizeof(float));
	s.write((char*)&width, sizeof(int));
	s.write((char*)&height, sizeof(int));
	s.write((char*)&range, sizeof(float));
	if (ver.cver.major >= 2)
		s.write((char*)&cycle, sizeof(float));
	return(!s.fail());
}

void RSW::SoundObject::Dump(std::ostream& o, const std::string& pfx) const {
	o << pfx << "SoundObject" << std::endl;
	o << pfx << "\tName: " << name << std::endl;
	o << pfx << "\tWave name: " << waveName << std::endl;
	o << pfx << "\tPos: " << pos[0] << ", " << pos[1] << ", " << pos[2] << std::endl;
	o << pfx << "\tVol: " << vol << std::endl;
	o << pfx << "\tWidth: " << width << std::endl;
	o << pfx << "\tHeight: " << height << std::endl;
	o << pfx << "\tRange: " << range << std::endl;
	o << pfx << "\tCycle: " << cycle << std::endl;
}

RSW::SoundObject& RSW::SoundObject::operator = (const SoundObject& obj) {
	memcpy(name, obj.name, sizeof(name));
	memcpy(waveName, obj.waveName, sizeof(waveName));
	memcpy(pos, obj.pos, sizeof(pos));
	vol = obj.vol;
	width = obj.width;
	height = obj.height;
	range = obj.range;
	cycle = obj.cycle;
	return(*this);
}


// ===== EFFECT
RSW::EffectObject::EffectObject() : Object(RSW::EffectType) {
}


RSW::EffectObject::EffectObject(const EffectObject& obj) : Object(RSW::EffectType) {
	*this = obj;
}

RSW::EffectObject::~EffectObject() {
}

bool RSW::EffectObject::readData(std::istream& s, const s_obj_ver& ver) {
	s.read(name, 80);
	name[79] = 0;
	s.read((char*)&pos, sizeof(float) * 3);
	s.read((char*)&type, sizeof(int));
	s.read((char*)&emitSpeed, sizeof(float));
	s.read((char*)&param, sizeof(float) * 4);
	return(!s.fail());
}

bool RSW::EffectObject::writeData(std::ostream& s, const s_obj_ver& ver) const {
	s.write(name, 80);
	s.write((char*)&pos, sizeof(float) * 3);
	s.write((char*)&type, sizeof(int));
	s.write((char*)&emitSpeed, sizeof(float));
	s.write((char*)&param, sizeof(float) * 4);
	return(!s.fail());
}

void RSW::EffectObject::Dump(std::ostream& o, const std::string& pfx) const {
	o << pfx << "EffectObject" << std::endl;
	o << pfx << "\tName: " << name << std::endl;
	o << pfx << "\tPos: " << pos[0] << ", " << pos[1] << ", " << pos[2] << std::endl;
	o << pfx << "\tType: " << type << std::endl;
	o << pfx << "\tEmit speed: " << emitSpeed << std::endl;
	o << pfx << "\tParam: " << param[0] << ", " << param[1] << ", " << param[2] << ", " << param[3] << std::endl;
}

RSW::EffectObject& RSW::EffectObject::operator = (const EffectObject& obj) {
	memcpy(name, obj.name, sizeof(name));
	memcpy(pos, obj.pos, sizeof(pos));
	type = obj.type;
	emitSpeed = obj.emitSpeed;
	memcpy(param, obj.param, sizeof(param));
	return(*this);
}

} /* namespace ro */
