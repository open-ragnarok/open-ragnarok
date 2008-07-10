/* $id$ */
#include "stdafx.h"
#include "rsw.h"
#include "ro.h"

RO::RSW::RSW() {
	object_count = 0;
}

RO::RSW::~RSW() {
	Clear();
}

bool RO::RSW::readStream(std::istream& s) {
	readHeader(s);

	if (!checkHeader(RSW_HEADER)) {
		std::cout << "Invalid RSW header (" << magic[0] << magic[1] << magic[2] << magic[3] << ")" << std::endl;
		return(false);
	}

	s.read(ini_file, 40);
	s.read(gnd_file, 40);
	if (IsCompatibleWith(1, 4))
		s.read(gat_file, 40);
	else
		memset(gat_file, 0, 40);
	s.read(scr_file, 40);

	// Water
	if (IsCompatibleWith(1,3))
		s.read((char*)&water.height, sizeof(float));
	else
		water.height = 0.0f;

	if( IsCompatibleWith(1,8) ) {
		s.read((char*)&water.type, sizeof(int) + sizeof(float) * 3);
		//water.type = readInt(s);
		//water.amplitude = readFloat(s);
		//water.phase = readFloat(s);
		//water.surface_curve_level = readFloat(s);
	}
	else {
		water.type = 0;
		water.amplitude = 1.0;
		water.phase = 2.0;
		water.surface_curve_level = 0.5;
	}
	if( IsCompatibleWith(1,9) )
		s.read((char*)&water.texture_cycling, sizeof(int));
	else
		water.texture_cycling = 3;

	if( IsCompatibleWith(1,4) ) {
		s.read((char*)&light, sizeof(strLight));
	}
	else {
		memset((char*)&light, 0, sizeof(strLight));
	}

	s.read((char*)unk, sizeof(int) * 3);

	s.read((char*)&object_count, sizeof(unsigned int));
	m_objects = new Object*[object_count];

	int objtype;
	Object* o;
	for (unsigned int i = 0; i < object_count; i++) {
		s.read((char*)&objtype, sizeof(int));
		switch (objtype) {
			case 1:
				o = new Model();
				break;
			case 2:
				o = new Light();
				break;
			case 3:
				o = new Sound();
				break;
			case 4:
				o = new Effect();
				break;
			default:
				fprintf(stderr, "ERROR! invalid object %u: %u\n", i, objtype);
				return(false);
		}
		o->readStream(s);
		m_objects[i] = o;
	}

	return(true);
}

bool RO::RSW::writeStream(std::ostream&) {
	return(true);
}

void RO::RSW::Dump(std::ostream& o) const {
	char buf[1024];
	sprintf(buf,"Magic: %c%c%c%c\n", magic[0], magic[1], magic[2], magic[3]);
	o << buf;
	sprintf(buf,"Version: %u.%u\n", m_version.cver.major, m_version.cver.minor);
	o << buf;
	sprintf(buf,"Ini: %s\n", ini_file);
	o << buf;
	sprintf(buf,"Gnd: %s\n", gnd_file);
	o << buf;
	sprintf(buf,"Gat: %s\n", gat_file);
	o << buf;
	sprintf(buf,"Scr: %s\n", scr_file);
	o << buf;
	sprintf(buf,"Water\n");
	o << buf;
	sprintf(buf,"\tHeight: %f\n", water.height);
	o << buf;
	sprintf(buf,"\tType: %u\n", water.type);
	o << buf;
	sprintf(buf,"\tAmpliture: %.2f\n", water.amplitude);
	o << buf;
	sprintf(buf,"\tPhase: %.2f\n", water.phase);
	o << buf;
	sprintf(buf,"\tSurface curve level: %.2f\n", water.surface_curve_level);
	o << buf;
	sprintf(buf,"Light\n");
	o << buf;
	sprintf(buf,"\tAmbient: %.2f %.2f %.2f\n", light.ambient[0], light.ambient[1], light.ambient[2]);
	o << buf;
	sprintf(buf,"\tDiffuse: %.2f %.2f %.2f\n", light.diffuse[0], light.diffuse[1], light.diffuse[2]);
	o << buf;
	sprintf(buf,"\tShadow: %.2f %.2f %.2f\n", light.shadow[0], light.shadow[1], light.shadow[2]);
	o << buf;
	sprintf(buf,"\tAlpha: %.2f\n", light.alpha);
	o << buf;
	sprintf(buf,"Unknown: %d\n", unk[0]);
	o << buf;
	sprintf(buf,"Unknown: %d\n", unk[1]);
	o << buf;
	sprintf(buf,"Unknown: %d\n", unk[2]);
	o << buf;
	sprintf(buf,"Objects: %u\n", object_count);
	o << buf;

	unsigned int i;
	for (i = 0; i < object_count; i++) {
		m_objects[i]->Dump(o);
	}
}

void RO::RSW::Clear() {
	if (object_count > 0) {
		for (unsigned int i = 0; i < object_count; i++) {
			delete m_objects[i];
		}
		delete[] m_objects;
	}
	object_count = 0;
}


// ===== OBJECT

RO::RSW::Object::Object(ObjectType t) {
	m_type = t;
}

RO::RSW::Object::~Object() {
}

RO::RSW::Object::ObjectType RO::RSW::Object::getType() const {
	return(m_type);
}

bool RO::RSW::Object::isType(ObjectType t) const {
	return(m_type == t);
}

void RO::RSW::Object::Dump(std::ostream& o) const {
	o << "No data to dump" << std::endl;
}

// ===== MODEL
RO::RSW::Model::Model() : Object(RO::RSW::Object::OT_Model) {
	memset(&m_data, 0, sizeof(ModelData));
	data = &m_data;
}

RO::RSW::Model::~Model() {
}

bool RO::RSW::Model::readStream(std::istream& s) {
	s.read((char*)&m_data, sizeof(ModelData));
	return(true);
}

bool RO::RSW::Model::writeStream(std::ostream& s) const {
	s.write((char*)&m_type, sizeof(int));
	s.write((char*)&m_data, sizeof(ModelData));
	return(true);
}

void RO::RSW::Model::Dump(std::ostream& o) const {
	char buf[512];
	o << "Model " << m_data.filename << std::endl;
	o << "\t" << m_data.m_name << std::endl;
	sprintf(buf, "pos: %.2f, %.2f, %.2f", m_data.pos[0], m_data.pos[1], m_data.pos[2]);
	o << "\t" << buf << std::endl;
	sprintf(buf, "rot: %.2f, %.2f, %.2f", m_data.rot[0], m_data.rot[1], m_data.rot[2]);
	o << "\t" << buf << std::endl;
	sprintf(buf, "scale: %.2f, %.2f, %.2f", m_data.scale[0], m_data.scale[1], m_data.scale[2]);
	o << "\t" << buf << std::endl;
}

// ===== LIGHT

RO::RSW::Light::Light() : Object(RO::RSW::Object::OT_Light) {
	memset(&m_data, 0, sizeof(LightData));
	data = &m_data;
}

RO::RSW::Light::~Light() {
}

bool RO::RSW::Light::readStream(std::istream& s) {
	s.read((char*)&m_data, sizeof(LightData));
	return(true);
}

bool RO::RSW::Light::writeStream(std::ostream& s) const {
	s.write((char*)&m_type, sizeof(int));
	s.write((char*)&m_data, sizeof(LightData));
	return(true);
}

// ===== SOUND
RO::RSW::Sound::Sound() : Object(RO::RSW::Object::OT_Sound) {
	memset(&m_data, 0, sizeof(SoundData));
	data = &m_data;
}

RO::RSW::Sound::~Sound() {
}

bool RO::RSW::Sound::readStream(std::istream& s) {
	s.read((char*)&m_data, sizeof(SoundData));
	return(true);
}

bool RO::RSW::Sound::writeStream(std::ostream& s) const {
	s.write((char*)&m_type, sizeof(int));
	s.write((char*)&m_data, sizeof(SoundData));
	return(true);
}

// ===== EFFECT

RO::RSW::Effect::Effect() : Object(RO::RSW::Object::OT_Effect) {
	memset(&m_data, 0, sizeof(EffectData));
	data = &m_data;
}

RO::RSW::Effect::~Effect() {
}

bool RO::RSW::Effect::readStream(std::istream& s) {
	s.read((char*)&m_data, sizeof(EffectData));
	return(true);
}

bool RO::RSW::Effect::writeStream(std::ostream& s) const {
	s.write((char*)&m_type, sizeof(int));
	s.write((char*)&m_data, sizeof(EffectData));
	return(true);
}