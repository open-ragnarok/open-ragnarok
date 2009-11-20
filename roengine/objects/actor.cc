#include "stdafx.h"

#include "roengine/objects/actor.h"

Actor::Actor() : GLObject() {
	map_x = map_y = 0;
	dest_x = dest_y = 0;
	m_act = 0;
	m_frame = 0;
	speed = 0.2f;
	m_dir = ro::DIR_N;
	m_type = UnknownType;
	id = 0;
	m_visible = true;
}

Actor::Actor(Type t) : GLObject() {
	map_x = map_y = 0;
	dest_x = dest_y = 0;
	m_act = 0;
	m_frame = 0;
	speed = 0.2f;
	m_dir = ro::DIR_N;
	m_type = t;
	id = 0;
	m_visible = true;
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
