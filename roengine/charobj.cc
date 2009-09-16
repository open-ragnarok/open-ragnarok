#include "stdafx.h"

#include "roengine/objects/charobj.h"

CharObj::CharObj() : GLObject() {
}

CharObj::~CharObj() {
}

void CharObj::Draw(RswObject* map, unsigned long ticks) {
	float wx, wy, wz;
	glPushMatrix();
	// Retrieves the world position based on map position
	map->getWorldPosition(map_x, map_y, &wx, &wy, &wz);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(wx, wy, wz); // Moves our object to the proper place
	m_actgl.Draw(ticks); // Draw
	glPopMatrix();
}

void CharObj::Draw() {
	if (m_actgl.valid())
		m_actgl.Draw();
}
