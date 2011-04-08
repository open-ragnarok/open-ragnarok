/* $Id$ */
#include "stdafx.h"

#include "roengine/ro_object_cache.h"
#include "ro/ro.h"

#include <sstream>

bool ROObjectCache::ReadRSM(const std::string& name, FileManager& fm) {
	if (exists(name)) {
		return(false);
	}

	FileData data = fm.getFile(name);
	if (data.blobSize() == 0)
		return(false);
	
	ro::RSM* rsm = new ro::RSM();
	std::stringstream ss;
	data.write(ss);
	if (!rsm->readStream(ss)) {
		delete(rsm);
		return(false);
	}
	add(name, rsm);
	return(true);
}

bool ROObjectCache::ReadRSW(const std::string& name, FileManager& fm, bool depedencies) {
	if (exists(name))
		return(false);

	FileData data = fm.getFile(name);
	if (data.blobSize() == 0)
		return(false);

	ro::RSW* rsw = new ro::RSW();
	std::stringstream ss;
	data.write(ss);
	if (!rsw->readStream(ss)) {
		delete(rsw);
		return(false);
	}
	add(name, rsw);

	if (depedencies) {
		std::string fn;

		fn = rsw->getGatFile();
		// ReadGAT(fn, fm);
		fn = rsw->getGndFile();
		ReadGND(fn, fm);

		std::string model_pfx = "model\\";
		unsigned int i;
		for (i = 0; i < rsw->getObjectCount(); i++) {
			const ro::RSW::ModelObject* obj = rsw->getModelObject(i);
			if (obj != NULL) {
				fn = model_pfx + obj->modelName;
				if (!exists(fn))
					if (!ReadRSM(fn, fm))
						std::cerr << "Error loading model " << fn << " for RSW " << name << std::endl;
			}
		}
	}

	return(true);
}

bool ROObjectCache::ReadGND(const std::string& name, FileManager& fm) {
	if (exists(name))
		return(false);

	FileData data = fm.getFile(name);
	if (data.blobSize() == 0)
		return(false);

	ro::GND* rsw = new ro::GND();
	std::stringstream ss;
	data.write(ss);
	if (!rsw->readStream(ss)) {
		delete(rsw);
		return(false);
	}
	add(name, rsw);
	return(true);
}

bool ROObjectCache::ReadGAT(const std::string& name, FileManager& fm) {
	if (exists(name))
		return(false);

	FileData data = fm.getFile(name);
	if (data.blobSize() == 0)
		return(false);

	ro::GAT* obj = new ro::GAT();
	std::stringstream ss;
	data.write(ss);
	if (!obj->readStream(ss)) {
		delete(obj);
		return(false);
	}
	add(name, obj);
	return(true);
}

bool ROObjectCache::ReadACT(const std::string& name, FileManager& fm) {
	if (exists(name))
		return(true);

	FileData data = fm.getFile(name);
	if (data.blobSize() == 0)
		return(false);

	ro::ACT* act = new ro::ACT();
	std::stringstream ss;
	data.write(ss);
	if (!act->readStream(ss)) {
		delete(act);
		return(false);
	}
	add(name, act);
	return(true);
}

bool ROObjectCache::ReadSPR(const std::string& name, FileManager& fm) {
	if (exists(name))
		return(false);

	FileData data = fm.getFile(name);
	if (data.blobSize() == 0)
		return(false);

	ro::SPR* spr = new ro::SPR();
	std::stringstream ss;
	data.write(ss);
	if (!spr->readStream(ss)) {
		delete(spr);
		return(false);
	}
	add(name, spr);
	return(true);
}

bool ROObjectCache::ReadSTR(const std::string& name, FileManager& fm) {
	if (exists(name))
		return(false);

	FileData data = fm.getFile(name);
	if (data.blobSize() == 0)
		return(false);

	ro::STR* str = new ro::STR();
	std::stringstream ss;
	data.write(ss);
	if (!str->readStream(ss)) {
		delete(str);
		return(false);
	}
	add(name, str);
	return(true);
}
