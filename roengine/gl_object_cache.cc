/* $Id$ */
#include "stdafx.h"

#include "roengine/gl_object_cache.h"

void GLObjectCache::draw(const Frustum* f, long tickdelay, RO::CDir camera_look) {
	obj_drawn = 0;
	obj_total = objects.size();

	obj_t::iterator itr = objects.begin();
	GLObject* obj;
	while(itr != objects.end()) {
		obj = itr->second;
		if (obj->isVisible()) {
			if (obj->isInFrustum(f)) {
				obj->Render(tickdelay, f, camera_look);
				obj_drawn++;
			}
		}
		itr++;
	}
}
