/* $Id$ */
#include "stdafx.h"

#include "file_manager.h"

FileData::FileData() : DynamicBlob() {
}

FileData::FileData(const FileData& o) : DynamicBlob(o) {
}

FileData::FileData(const char* data, unsigned int size) : DynamicBlob(size) {
	memcpy(this->buffer, data, size);
}

bool FileData::write(std::ostream& os) const {
	os.write(this->buffer, dataSize);
	return(true);
}

bool FileLoader::writeFile(const std::string& name, std::ostream& os) {
	FileData file = getFile(name);
	return(file.write(os));
}

bool GRFFileLoader::fileExists(const std::string& name) const {
	std::string fn;
	for (unsigned int i = 0; i < name.length(); i++)
		if (name[i] > 'A' && name[i] < 'Z')
			fn += (name[i] - 'A' + 'a');
		else
			fn += name[i];

	return(m_grf.fileExists(fn));
}

FileData GRFFileLoader::getFile(const std::string& name) {
	FileData ret;
	std::stringstream ss;
	std::string fn;
	for (unsigned int i = 0; i < name.length(); i++)
		if (name[i] > 'A' && name[i] < 'Z')
			fn += (name[i] - 'A' + 'a');
		else
			fn += name[i];

	m_grf.write(fn, ss);
	ret.setSize(ss.tellp());
	ss.read(ret.getBuffer(), ss.tellp());
	return(ret);
}

bool GRFFileLoader::open(const std::string& name) {
	return(m_grf.open(name));
}


FileManager::FileManager() {
}

FileManager::FileManager(const std::string& ini) {
	// Reads information from an INI file
	LoadIni(ini);
}

bool FileManager::LoadIni(const std::string& ini) {
	return(true);
}

bool FileManager::add(const std::string& name, FileLoader* loader) {
	if (!BaseCache::add(name, loader))
		return(false);
	LoadOrder.push_back(name);
	return(true);
}

bool FileManager::remove(const std::string& name) {
	if (!BaseCache::remove(name))
		return(false);
	std::vector<std::string>::iterator itr;
	itr = LoadOrder.begin();
	while(itr != LoadOrder.end()) {
		if ((*itr) == name) {
			LoadOrder.erase(itr);
			return(true);
		}
		itr++;
	}

	return(true);
}

bool FileManager::OpenGRF(const std::string& grf_fn) {
	std::string grfn = "grf:";
	grfn += grf_fn;

	if (exists(grfn))
		return(false);

	GRFFileLoader *m_grf = new GRFFileLoader();
	if (!m_grf->open(grf_fn)) {
		delete(m_grf);
		return(false);
	}

	add(grfn, m_grf);
	LoadOrder.push_back(grfn);
	return(true);
}

bool FileManager::fileExists(const std::string& name) const {
	std::vector<std::string>::const_iterator itr;
	itr = LoadOrder.begin();
	const FileLoader *fl;
	while (itr != LoadOrder.end()) {
		fl = get(*itr);
		if (fl->fileExists(name))
			return(true);
		itr++;
	}
	return(false);
}

FileData FileManager::getFile(const std::string& name) {
	std::vector<std::string>::const_iterator itr;
	FileData ret;
	itr = LoadOrder.begin();
	FileLoader *fl;
	while (itr != LoadOrder.end()) {
		fl = get(*itr);
		if (fl->fileExists(name)) {
			ret = fl->getFile(name);
			return(ret);
		}
		itr++;
	}

	return(ret);
}
