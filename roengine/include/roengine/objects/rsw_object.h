/* $Id$ */
#ifndef __RSW_OBJECT_H
#define __RSW_OBJECT_H

#include "../gl_object.h"
#include "ro/types/rsw.h"
#include "ro/types/gat.h"
#include "ro/types/gnd.h"
#include "../cache_manager.h"
#include "../texturecache.h"

/**
 * Stores the data of the map and draws it as requested.
 */
class RswObject : public GLObject {
protected:
	static float m_tilesize;
	const RO::RSW* rsw;
	const RO::GND* gnd;
	const RO::GAT* gat;
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
	bool loadTextures(CacheManager&);

	virtual void Draw();

	/**
	 * Draws the map, and scans the map for the 3D position of the mouse given the screen pointer position
	 */
	void DrawRSW(int screen_x, int screen_y);
	void DrawSelection(int mapx, int mapy) const;
	virtual bool isInFrustum(const Frustum&) const;

	/**
	 * Retrieves the GL position (rx, ry, rz) of the given map coordinates
	 * @param mapx map X coordinate
	 * @param mapy map Y coordinate
	 * @param rx float world X coordinate
	 * @param ry float world Z coordinate
	 * @param ry float world Z coordinate
	 */
	void getWorldPosition(int mapx, int mapy, float *rx, float *ry, float *rz);

	/**
	 * Retrieves the RSW file used to draw this map
	 */
	const RO::RSW* getRSW() const;

	/**
	 * Retrieves the GND file used to draw this map
	 */
	const RO::GND* getGND() const;

	bool valid() const;

	static RswObject* open(CacheManager&, const char* map);

	int getMouseMapX() const;
	int getMouseMapY() const;

	float getWorldX() const;
	float getWorldY() const;
	float getWorldZ() const;
};

#endif /* __RSW_OBJECT_H */
