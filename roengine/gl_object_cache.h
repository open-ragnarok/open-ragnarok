/* $Id$ */
#ifndef __GL_OBJECT_CACHE_H
#define __GL_OBJECT_CACHE_H

#include "base_cache.h"
#include "gl_object.h"

#include "objects/cube_object.h"
#include "objects/rsm_object.h"
#include "objects/rsw_object.h"
#include "frustum.h"

class GLObjectCache : public BaseCache<GLObject> {
public:
	int obj_drawn, obj_total;
	void draw(const Frustum&, long tickdelay = 0);
};

#endif /* __GL_OBJECT_CACHE_H */
