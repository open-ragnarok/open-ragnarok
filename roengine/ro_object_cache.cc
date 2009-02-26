/* $Id$ */
#include "stdafx.h"

#include "ro_object_cache.h"
#include "ro.h"

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

bool ROObjectCache::ReadRSW(const std::string& name, FileManager& fm, bool depedencies) {
	if (exists(name))
		return(false);

	FileData data = fm.getFile(name);
	if (data.blobSize() == 0)
		return(false);

	RO::RSW* rsw = new RO::RSW();
	std::stringstream ss;
	data.write(ss);
	if (!rsw->readStream(ss)) {
		delete(rsw);
		return(false);
	}
	add(name, rsw);

	if (depedencies) {
		std::string fn;

		fn = rsw->gat_file;
		// ReadGAT(fn, fm);
		fn = rsw->gnd_file;
		ReadGND(fn, fm);

		std::string model_pfx = "model\\";
		unsigned int i;
		for (i = 0; i < rsw->getObjectCount(); i++) {
			if (rsw->getObject(i)->isType(RO::RSW::OT_Model)) {
				fn = model_pfx + ((RO::RSW::Model*)rsw->getObject(i))->data->filename;
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

	RO::GND* rsw = new RO::GND();
	std::stringstream ss;
	data.write(ss);
	if (!rsw->readStream(ss)) {
		delete(rsw);
		return(false);
	}
	add(name, rsw);
	return(true);
}

bool ROObjectCache::ReadACT(const std::string& name, FileManager& fm) {
	if (exists(name))
		return(false);

	FileData data = fm.getFile(name);
	if (data.blobSize() == 0)
		return(false);

	RO::ACT* act = new RO::ACT();
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

	RO::SPR* spr = new RO::SPR();
	std::stringstream ss;
	data.write(ss);
	if (!spr->readStream(ss)) {
		delete(spr);
		return(false);
	}
	add(name, spr);
	return(true);
}
