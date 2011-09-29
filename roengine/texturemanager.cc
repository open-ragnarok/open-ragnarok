/* $Id$ */
#include "stdafx.h"

#include "roengine/texturemanager.h"

#include "sdle/image.h"
#include "sdle/image_bmp.h"
#include "sdle/texture_png.h"
#include "sdle/texture_jpeg.h"
#include "sdle/texture_tga.h"


// === TEXTURE MANAGER ===

TextureManager::TextureManager() {
}

TextureManager::~TextureManager() {
	Clear();
}

void TextureManager::Clear() {
	textures.clear();
}

sdle::Texture TextureManager::RegisterJPEG(FileManager& fm, const std::string& name) {
	if (IsRegistered(name))
		return(textures[name]);

	sdle::Texture t;
	FileData d = fm.getFile(name);
	if (d.blobSize() == 0) {
		return(t);
	}

	t = sdle::loadJPEGTexture((unsigned char*)d.getBuffer(), d.blobSize());
	if (t.Valid())
		Register(name, t);
	
	return(t);
}

sdle::Texture TextureManager::RegisterPNG(FileManager& fm, const std::string& name) {
	if (IsRegistered(name))
		return(textures[name]);

	sdle::Texture t;
	FileData d = fm.getFile(name);
	if (d.blobSize() == 0) {
		return(t);
	}

	t = sdle::loadPngTexture(d.getBuffer(), d.blobSize());
	if (t.Valid())
		Register(name, t);
	
	return(t);
}

sdle::Texture TextureManager::RegisterTGA(FileManager& fm, const std::string& name) {
	if (IsRegistered(name))
		return(textures[name]);

	sdle::Texture t;
	FileData d = fm.getFile(name);
	if (d.blobSize() == 0) {
		return(t);
	}

	t = sdle::loadTGATexture((unsigned char*)d.getBuffer(), d.blobSize());
	if (t.Valid())
		Register(name, t);
	
	return(t);
}

sdle::Texture TextureManager::Register(FileManager& fm, const std::string& name) {
	if (!fm.fileExists(name)) {
		std::cerr << "Can't load texture " << name << std::endl;
		return(sdle::Texture());
	}

	const char* fn = name.c_str();
	const char* extension = fn + (strlen(fn) - 3);

	if (!strcmp(extension, "jpg")) {
		return(RegisterJPEG(fm, name));
	}

	if (!strcmp(extension, "png")) {
		return(RegisterPNG(fm, name));
	}

	if (!strcmp(extension, "tga")) {
		return(RegisterTGA(fm, name));
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

	sdle::Texture ret(r);

	textures[name] = ret;

	return(ret);
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
	const ro::SPR* spr;
	rogl::Texture::PointerCache ret;

	if (!cache.ReadSPR(name,fm))
		return(ret);
	spr = (const ro::SPR*)cache[name];

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
