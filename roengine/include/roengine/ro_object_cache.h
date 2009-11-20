/* $Id$ */
#ifndef __RO_OBJECT_CACHE_H
#define __RO_OBJECT_CACHE_H

#include "base_cache.h"
#include "file_manager.h"
#include "ro/ro_object.h"

#include <string>

/**
 * Reads and stores in memory basic interface objects
 */
class ROObjectCache : public BaseCache<ro::Object> {
public:
	/** Reads an RSM Format from the FileManager and stores it in cache (if loade is successful) */
	bool ReadRSM(const std::string& name, FileManager& fm);
	bool ReadRSW(const std::string& name, FileManager& fm, bool depedencies = false);
	bool ReadGND(const std::string& name, FileManager& fm);
	bool ReadACT(const std::string& name, FileManager& fm);
	bool ReadSPR(const std::string& name, FileManager& fm);
	bool ReadGAT(const std::string& name, FileManager& fm);
};

#endif /* __RO_OBJECT_CACHE_H */
