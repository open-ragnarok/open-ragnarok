/* $Id$ */
#ifndef __GL_OBJECT_CACHE_H
#define __GL_OBJECT_CACHE_H

#include "base_cache.h"
#include "gl_object.h"

#include "objects/cube_object.h"
#include "objects/rsm_object.h"
#include "objects/rsw_object.h"

class GLObjectCache : public BaseCache<GLObject> {
public:
	void draw(long tickdelay = 0);
};

#endif /* __GL_OBJECT_CACHE_H */
