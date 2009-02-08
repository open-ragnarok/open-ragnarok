/* $Id$ */
#include "stdafx.h"

#include "gl_object_cache.h"

void GLObjectCache::draw(const Frustum& f, long tickdelay) {
	obj_drawn = 0;
	obj_total = objects.size();

	obj_t::iterator itr = objects.begin();
	GLObject* obj;
	while(itr != objects.end()) {
		obj = itr->second;
		if (obj->isVisible()) {
			if (obj->isInFrustum(f)) {
				obj->Render(tickdelay);
				obj_drawn++;
			}
		}
		itr++;
	}
}