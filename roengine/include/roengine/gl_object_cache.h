/* $Id$ */
#ifndef __GL_OBJECT_CACHE_H
#define __GL_OBJECT_CACHE_H

#include "base_cache.h"
#include "gl_object.h"

#include "roengine/frustum.h"
#include "roengine/vector.h"

class GLObjectCache : public BaseCache<GLObject> {
public:
	int obj_drawn, obj_total;
	void draw(const Frustum&, long tickdelay = 0, const Vector3f& camera_look = Vector3f::UNIT_Z);
};

#endif /* __GL_OBJECT_CACHE_H */
