/* $Id$ */
#ifndef __FILE_MANAGER_H
#define __FILE_MANAGER_H

#include "base_cache.h"
#include "blob.h"

#include <vector>

class FileData : public DynamicBlob<char> {
public:
	FileData();
	FileData(const FileData& o);
	FileData(const char* data, unsigned int size);
	bool write(std::ostream& os) const;
};

class FileLoader {
public:
	virtual bool fileExists(const std::string& name) const = 0;
	virtual FileData getFile(const std::string& name) = 0;
	virtual bool open(const std::string& name) = 0;

	bool writeFile(const std::string& name, std::ostream& os);
};

#include "grf.h"
// Reads files from a GRF file
class GRFFileLoader : public FileLoader {
protected:
	RO::GRF m_grf;
public:
	virtual bool fileExists(const std::string& name) const;
	virtual FileData getFile(const std::string& name);
	virtual bool open(const std::string& name);
};

// Reads files from a filesystem directory
class FSFileLoader : public FileLoader {
};

// Reads files from a Zip file
class ZIPFileLoader : public FileLoader {
};

class FileManager : public BaseCache<FileLoader> {
protected:
	std::vector<std::string> LoadOrder;
public:
	FileManager();
	FileManager(const std::string& ini);
	bool LoadIni(const std::string& ini);

	bool add(const std::string& name, FileLoader* loader);
	bool remove(const std::string& name);

	bool OpenGRF(const std::string& grf_fn);

	bool fileExists(const std::string& name) const;
	FileData getFile(const std::string& name);
};

#endif