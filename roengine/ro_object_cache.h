/* $Id$ */
#ifndef __RO_OBJECT_CACHE_H
#define __RO_OBJECT_CACHE_H

#include "base_cache.h"
#include "file_manager.h"
#include "ro_object.h"

#include <string>

class ROObjectCache : public BaseCache<RO::Object> {
public:
	bool ReadRSM(const std::string& name, FileManager& fm);
};

#endif /* __RO_OBJECT_CACHE_H */
