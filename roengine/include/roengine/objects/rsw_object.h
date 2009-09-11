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

class RswObject : public GLObject {
protected:
	const RO::RSW* rsw;
	const RO::GND* gnd;
	TextureCache textures;

	void DrawGND();

	unsigned int gnd_gl;

public:
	RswObject(const RO::RSW*, ROObjectCache&);
	virtual ~RswObject();

	bool loadTextures(TextureManager&, FileManager&);

	virtual void Draw();
	virtual bool isInFrustum(const Frustum&) const;
};

#endif /* __RSW_OBJECT_H */
