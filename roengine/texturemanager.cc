/* $Id$ */
#include "stdafx.h"

#include "roengine/texturemanager.h"

#include "sdle/image.h"
#include "sdle/image_bmp.h"

#include <GL/gl.h>
#include <GL/glu.h>

// === TEXTURE MANAGER ===

TextureManager::TextureManager() {
}

TextureManager::~TextureManager() {
	Clear();
}

void TextureManager::Clear() {
	textures.clear();
}

sdle::Texture TextureManager::Register(FileManager& fm, const std::string& name) {
	if (!fm.fileExists(name)) {
		std::cerr << "Can't load texture " << name << std::endl;
		return(sdle::Texture());
	}

	if (IsRegistered(name))
		return(textures[name]);

	FileData data = fm.getFile(name);
	std::stringstream imgfile;
	data.write(imgfile);
	sdle::ImageBMP img(imgfile);
	return(Register(name, img));
}

sdle::Texture TextureManager::Register(const std::string& name, const sdle::Image& img) {
	if (IsRegistered(name))
		return(textures[name]);

	sdle::Texture::Root* r = new sdle::Texture::Root();
	if (r->Create(&img) == false) {
		std::cerr << "Can't store texture in memory " << name << std::endl;
		delete(r);
		return(sdle::Texture());
	}

	return(sdle::Texture(r));
}

sdle::Texture TextureManager::Register(const std::string& name, const sdle::Texture& t) {
	textures[name] = t;
	return(t);
}


bool TextureManager::UnRegister(const std::string& name) {
	if (!IsRegistered(name))
		return(false);
	tex_t::iterator itr = textures.find(name);
	textures.erase(itr);

	return(true);
}

bool TextureManager::IsRegistered(const std::string& name) const {
	if (textures.find(name) == textures.end())
		return(false);
	return(true);
}

bool TextureManager::Activate(const std::string& name) const {
	tex_t::const_iterator itr = textures.find(name);
	if (itr == textures.end())
		return(false);

	if (!itr->second.Activate())
		return(false);

	return(true);
}


sdle::Texture TextureManager::operator [](const std::string& name) const {
	tex_t::const_iterator itr = textures.find(name);
	if (itr == textures.end())
		return(sdle::Texture());
	return(itr->second);
}

/*
rogl::Texture::PointerCache TextureManager::RegisterSPR(FileManager& fm, ROObjectCache& cache, const std::string& name) {
	const RO::SPR* spr;
	rogl::Texture::PointerCache ret;

	if (!cache.ReadSPR(name,fm))
		return(ret);
	spr = (const RO::SPR*)cache[name];

	for (unsigned int i = 0; i < spr->getImgCount(); i++) {
		char buf[256];
		sprintf(buf, "%s-%d", name.c_str(), i);
		rogl::ImageSPR img(spr, i);
		ret.add(Register(buf, img));
	}

	// We don't need it once we registered the textures.
	cache.remove(name);
	return(ret);
}
*/
