/* $Id$ */
#ifndef __TEXTUREMANAGER_H
#define __TEXTUREMANAGER_H

#include "file_manager.h"
#include "ro_object_cache.h"
#include "rogl/image.h"
#include "rogl/texture.h"

#include <string>
#include <map>
#include <list>

class TextureManager {
protected:
	typedef std::map<std::string, rogl::Texture*> tex_t;
	tex_t textures;
public:
	TextureManager();
	~TextureManager();

	void Clear();

	rogl::Texture::Pointer Register(FileManager&, const std::string& name);
	rogl::Texture::Pointer Register(const std::string& name, const rogl::Image& img);
	rogl::Texture::PointerCache RegisterSPR(FileManager&, ROObjectCache&, const std::string& name);
	bool UnRegister(const std::string& name);
	bool IsRegistered(const std::string& name) const;
	bool Activate(const std::string& name) const;

	rogl::Texture::Pointer operator [](const std::string& name) const;
};

#endif /* __TEXTUREMANAGER_H */
