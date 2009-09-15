/* $Id$ */
#ifndef __RSW_OBJECT_H
#define __RSW_OBJECT_H

#include "../gl_object.h"
#include "ro/types/rsw.h"
#include "ro/types/gnd.h"
#include "../texturemanager.h"
#include "../file_manager.h"
#include "../ro_object_cache.h"
#include "../texturecache.h"

/**
 * Stores the data of the map and draws it as requested.
 */
class RswObject : public GLObject {
protected:
	const RO::RSW* rsw;
	const RO::GND* gnd;
	TextureCache textures;

	void DrawGND();

	unsigned int gnd_gl;

	float world_x;
	float world_y;
	float world_z;

public:
	RswObject(const RO::RSW*, ROObjectCache&);
	virtual ~RswObject();

	/**
	 * Loads all textures needed by the map
	 */
	bool loadTextures(TextureManager&, FileManager&);

	virtual void Draw();

	/**
	 * Draws the map, and scans the map for the 3D position of the mouse given the screen pointer position
	 */
	void DrawRSW(int screen_x, int screen_y);
	void DrawSelection(int mapx, int mapy) const;
	virtual bool isInFrustum(const Frustum&) const;

	float getWorldX() const;
	float getWorldY() const;
	float getWorldZ() const;
};

#endif /* __RSW_OBJECT_H */
