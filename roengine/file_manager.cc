/* $Id$ */
#include "stdafx.h"

#include "file_manager.h"

#include <sys/stat.h>

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
	std::string fn, aux = "data\\";
	for (unsigned int i = 0; i < name.length(); i++)
		if (name[i] > 'A' && name[i] < 'Z')
			fn += (name[i] - 'A' + 'a');
		else
			fn += name[i];

	fn = aux + fn;

	return(m_grf.fileExists(fn));
}

FileData GRFFileLoader::getFile(const std::string& name) {
	FileData ret;
	std::stringstream ss;
	std::string fn, aux = "data\\";
	for (unsigned int i = 0; i < name.length(); i++)
		if (name[i] > 'A' && name[i] < 'Z')
			fn += (name[i] - 'A' + 'a');
		else
			fn += name[i];

	fn = aux + fn;

	m_grf.write(fn, ss);
	ret.setSize(ss.tellp());
	ss.read(ret.getBuffer(), ss.tellp());
	return(ret);
}

bool GRFFileLoader::open(const std::string& name) {
	return(m_grf.open(name));
}

bool FSFileLoader::fileExists(const std::string& name) const {
	std::string fn = m_path;
	if (fn[fn.length() - 1] != '\\')
		fn += "\\";
	fn += name;

	struct stat stFileInfo;
	// Stat returns 0 on success (meaning the file exists)
	if (stat(fn.c_str(), &stFileInfo))
		return(false);

	return(true);
}

FileData FSFileLoader::getFile(const std::string& name) {
	std::ifstream file;
	FileData ret;

	std::string fn = m_path;
	if (fn[fn.length() - 1] != '\\')
		fn += "\\";
	fn += name;

	file.open(fn.c_str(), std::ios_base::in | std::ios_base::binary);

	if(!file.is_open())
		return(ret);

	file.seekg(0, std::ios_base::end);
	int size = file.tellg();
	file.seekg(0, std::ios_base::beg);
	ret.setSize(size);
	file.read(ret.getBuffer(), size);

	return(ret);
}
bool FSFileLoader::open(const std::string& name) {
	m_path = name;
	return(true);
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

bool FileManager::OpenFS(const std::string& location) {
	std::string fnn = "fs:";
	fnn += location;

	if (exists(fnn))
		return(false);

	FSFileLoader *m_fs = new FSFileLoader();
	if (!m_fs->open(location)) {
		delete(m_fs);
		return(false);
	}

	add(fnn, m_fs);
	LoadOrder.push_back(fnn);
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
