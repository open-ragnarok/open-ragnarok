/* $Id$ */
#ifndef __TEXTUREMANAGER_H
#define __TEXTUREMANAGER_H

#include "file_manager.h"

#include <string>
#include <map>

class TextureManager {
protected:
	typedef std::map<std::string, unsigned int> tex_t;
	tex_t textures;
public:
	TextureManager();
	~TextureManager();

	void Clear();

	unsigned int Register(FileManager&, const std::string& name);
	bool UnRegister(const std::string& name);
	bool IsRegistered(const std::string& name) const;
	bool Activate(const std::string& name) const;

	unsigned int operator [](const std::string& name) const;
};

#endif /* __TEXTUREMANAGER_H */