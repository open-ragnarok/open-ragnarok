/* $Id$ */
#include "stdafx.h"

#include "roengine/spr_cache.h"

SprCache::SprCache() {}
SprCache::~SprCache() {}

bool SprCache::Load(const std::string& fn, ROObjectCache& objs, FileManager& fm, const ro::PAL* pal) {
	if (exists(fn))
		return(false);

	if (!objs.exists(fn)) {
		if (!objs.ReadSPR(fn, fm)) {
			return(false);
		}
	}

	rogl::SprGL* spr = new rogl::SprGL();
	if (!spr->open((ro::SPR*)objs[fn], pal)) {
		delete(spr);
		return(false);
	}

	add(fn, spr);

	return(true);
}
