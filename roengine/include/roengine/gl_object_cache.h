/* $Id$ */
#ifndef __GL_OBJECT_CACHE_H
#define __GL_OBJECT_CACHE_H

#include "base_cache.h"
#include "gl_object.h"

#include "roengine/frustum.h"
#include "roengine/vector.h"
#include "ro/constants.h"

class GLObjectCache : public BaseCache<GLObject> {
public:
	int obj_drawn, obj_total;
	void draw(const Frustum*, long tickdelay = 0, ro::CDir cameradir = ro::DIR_N);
};

#endif /* __GL_OBJECT_CACHE_H */
