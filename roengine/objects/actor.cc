#include "stdafx.h"

#include "roengine/objects/actor.h"

Actor::Actor() : GLObject() {
	map_x = map_y = 0;
	dest_x = dest_y = 0;
	m_act = 0;
	m_frame = 0;
	speed = 0.2f;
//	moveticks = 0;
	m_dir = ro::DIR_S;
	m_type = UnknownType;
	id = 0;
	m_visible = true;

	m_emotion = -1;
}

Actor::Actor(Type t) : GLObject() {
	map_x = map_y = 0;
	dest_x = dest_y = 0;
	m_act = 0;
	m_frame = 0;
	speed = 0.2f;
//	moveticks = 0;
	m_dir = ro::DIR_S;
	m_type = t;
	id = 0;
	m_visible = true;

	m_emotion = -1;
}

Actor::~Actor() {
}

bool Actor::valid() const {
	return(false);
}

void Actor::setMap(RswObject* map) {
	m_map = map;
}

float Actor::getPositionX() const {
	return(map_x);
}

float Actor::getPositionY() const {
	return(map_y);
}

float Actor::getDestinationX() const {
	return(dest_x);
}

float Actor::getDestinationY() const {
	return(dest_y);
}

void Actor::setDirection(ro::CDir dir) {
	m_dir = dir;
}

void Actor::setPos(float x, float y) {
	map_x = x;
	map_y = y;
	dest_x = x;
	dest_y = y;
}

void Actor::setDest(float x, float y) {
	dest_x = x;
	dest_y = y;
}

void Actor::setVisible(bool v) {
	m_visible = v;
}

bool Actor::isVisible() const {
	return(m_visible);
}

Actor::Type Actor::getType() const {
	return(m_type);
}

void Actor::setSpeed(unsigned short value) {
	speed = value;
}

void Actor::setAction(unsigned short action) {
	m_act = action;
}

void Actor::setEmotion(int emotion) {
	m_emotion = emotion;

	m_emotionact.Stop();
	m_emotionact.Play(false);
}

//bool Actor::shadowLoaded = false;

bool Actor::openAct(CacheManager& cache, std::string name, rogl::ActGL& actor) {
	//Cache objects
	ROObjectCache& objects = cache.getROObjects();
	GLObjectCache& globjects = cache.getGLObjects();
	TextureManager& tm = cache.getTextureManager();
	FileManager& fm = cache.getFileManager();

	std::string act_n, spr_n;

	rogl::SprGL sprgl;

	// Setup filenames
	act_n = name + ".act";
	spr_n = name + ".spr";

	// Reads the ACT object
	if (!objects.ReadACT(act_n, fm)) {
		fprintf(stderr, "Error loading act file %s.\n", act_n.c_str());
		return(false);
	}

	// Reads the SPR object
	if (!cache.getSprGLObjects().exists(spr_n)) {
		if (!cache.getSprGLObjects().Load(spr_n, objects, fm)) {
			fprintf(stderr, "Error opening SPR file");
			return(false);
		}
	}

	sprgl = *cache.getSprGLObjects().get(spr_n);

	// Creates an ActGL and registers it
	actor.setSpr(sprgl);
	actor.setAct((ro::ACT*)objects[act_n]);

	return(true);
}
