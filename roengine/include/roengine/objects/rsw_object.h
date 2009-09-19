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
	/** The size of the tile (default: 10.0f) */
	static float m_tilesize;

	const RO::RSW* rsw;
	const RO::GND* gnd;
	const RO::GAT* gat;

	/** Textures used by the ground */
	TextureCache textures;

	/** Water textures */
	TextureCache water_tex;

	/** Draws the Ground */
	void DrawGND();

	/** Draws the water */
	void DrawWater(unsigned int delay);

	unsigned int gnd_gl;

	/** The GLList used to draw the water */
	unsigned int m_watergl;

	/** The current water frame to draw */
	unsigned int m_waterframe;

	/** The ticks passed since last frame changed */
	unsigned int m_waterdelay;

	float world_x;
	float world_y;
	float world_z;

	static void getRot(float sizex, float sizey, float rot[16]);

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
	void DrawRSW(int screen_x, int screen_y, unsigned int delay);
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

	/**
	 * Reads a file from the cache and returns a new RswObject pointer to it.
	 *
	 * @param cache CacheManager to read stuff from
	 * @param map the map name to load (without extension)
	 * @return a New RswObject pointer on success or NULL on failure
	 */
	static RswObject* open(CacheManager&, const char* map);

	int getMouseMapX() const;
	int getMouseMapY() const;

	float getWorldX() const;
	float getWorldY() const;
	float getWorldZ() const;
};

#endif /* __RSW_OBJECT_H */
