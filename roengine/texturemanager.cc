/* $Id$ */
#include "stdafx.h"

#include "texturemanager.h"

#include "rogl/texture.h"
#include "rogl/image_bmp.h"
#include "rogl/image_spr.h"

// === TEXTURE MANAGER ===

TextureManager::TextureManager() {
}

TextureManager::~TextureManager() {
	Clear();
}

void TextureManager::Clear() {
	tex_t::iterator itr = textures.begin();
	rogl::Texture* tp;

	unsigned int glTex;
	while (itr != textures.end()) {
		tp = itr->second;
		glTex = tp->getIdx();
		//std::cout << "Unregistering texture " << itr->first << std::endl;
		delete(tp);
		itr++;
	}
	textures.clear();
}

rogl::Texture::Pointer TextureManager::Register(FileManager& fm, const std::string& name) {
	if (!fm.fileExists(name)) {
		std::cerr << "Can't load texture " << name << std::endl;
		return(0);
	}

	if (IsRegistered(name))
		return(textures[name]);

	FileData data = fm.getFile(name);
	std::stringstream imgfile;
	data.write(imgfile);
	rogl::ImageBMP img(imgfile);
	return(Register(name, img));
}

rogl::Texture::Pointer TextureManager::Register(const std::string& name, const rogl::Image& img) {
	if (IsRegistered(name))
		return(textures[name]);

	textures[name] = new rogl::Texture(img, name);
	rogl::Texture::Pointer tp(textures[name]);

	return(tp);
}

bool TextureManager::UnRegister(const std::string& name) {
	if (!IsRegistered(name))
		return(false);
	tex_t::iterator itr = textures.find(name);

	unsigned int glTex = itr->second->getIdx();
	glDeleteTextures(1, &glTex);
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

	if (!glIsTexture(itr->second->getIdx()))
		return(false);

	glBindTexture(GL_TEXTURE_2D, itr->second->getIdx());
	return(true);
}


rogl::Texture::Pointer TextureManager::operator [](const std::string& name) const {
	tex_t::const_iterator itr = textures.find(name);
	if (itr == textures.end())
		return(0);
	rogl::Texture::Pointer ptr(itr->second);
	return(ptr);
}

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