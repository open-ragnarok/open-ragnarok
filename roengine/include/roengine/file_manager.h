/* $Id$ */
#ifndef __FILE_MANAGER_H
#define __FILE_MANAGER_H

#include "base_cache.h"
#include "file_data.h"

#include <vector>

class FileLoader {
public:
	FileLoader();
	virtual ~FileLoader();
	virtual bool fileExists(const std::string& name) const = 0;
	virtual FileData getFile(const std::string& name) = 0;
	virtual bool open(const std::string& name) = 0;

	bool writeFile(const std::string& name, std::ostream& os);
};

#include "ro/types/grf.h"
// Reads files from a GRF file
class GRFFileLoader : public FileLoader {
protected:
	ro::GRF m_grf;

public:
	virtual bool fileExists(const std::string& name) const;
	virtual FileData getFile(const std::string& name);
	virtual bool open(const std::string& name);
};

// Reads files from a filesystem directory
class FSFileLoader : public FileLoader {
protected:
#ifdef _WIN32
	static const char DIR_SEPARATOR = '\\';
#else
	static const char DIR_SEPARATOR = '/';
#endif
	std::string m_path;
public:
	virtual bool fileExists(const std::string& name) const;
	virtual FileData getFile(const std::string& name);
	virtual bool open(const std::string& name);
};

// Reads files from a Zip file
class ZIPFileLoader : public FileLoader {
};

class FileManager : public BaseCache<FileLoader> {
protected:
	std::vector<std::string> LoadOrder;

	std::map<std::string, std::string> nametable;
public:
	FileManager();
	FileManager(const std::string& ini);
	bool LoadIni(const std::string& ini);

	bool add(const std::string& name, FileLoader* loader);
	bool remove(const std::string& name);

	bool OpenGRF(const std::string& grf_fn);
	bool OpenFS(const std::string& location);

	bool fileExists(const std::string& name) const;
	FileData getFile(const std::string& name);

	void addName(const std::string&, const std::string&);
};

#endif
