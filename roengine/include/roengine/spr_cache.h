/* $Id$ */
#ifndef __ROENGINE_SPR_CACHE_H
#define __ROENGINE_SPR_CACHE_H

#include "base_cache.h"
#include "ro_object_cache.h"
#include "rogl/sprgl.h"

class SprCache : public BaseCache<rogl::SprGL> {
public:
	SprCache();
	virtual ~SprCache();

	bool Load(const std::string&, ROObjectCache&, FileManager&);
};

#endif /* __ROENGINE_SPR_CACHE_H */
