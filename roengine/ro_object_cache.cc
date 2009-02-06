/* $Id$ */
#include "stdafx.h"

#include "ro_object_cache.h"
#include "rsm.h"

#include <sstream>

bool ROObjectCache::ReadRSM(const std::string& name, FileManager& fm) {
	if (exists(name)) {
		return(false);
	}

	FileData data = fm.getFile(name);
	if (data.blobSize() == 0)
		return(false);
	
	RO::RSM* rsm = new RO::RSM();
	std::stringstream ss;
	data.write(ss);
	if (!rsm->readStream(ss)) {
		delete(rsm);
		return(false);
	}
	add(name, rsm);
	return(true);
}