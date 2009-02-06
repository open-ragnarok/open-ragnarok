/* $Id$ */
#include "stdafx.h"

#include "gl_object_cache.h"

void GLObjectCache::draw(long tickdelay) {
	obj_t::iterator itr = objects.begin();
	GLObject* obj;
	while(itr != objects.end()) {
		obj = itr->second;
		if (obj->isVisible())
			obj->Render(tickdelay);
		itr++;
	}
}