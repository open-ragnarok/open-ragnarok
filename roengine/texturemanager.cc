/* $Id$ */
#include "stdafx.h"

#include "texturemanager.h"

#include "image_bmp.h"

TextureManager::TextureManager() {
}

TextureManager::~TextureManager() {
	Clear();
}

void TextureManager::Clear() {
	tex_t::iterator itr = textures.begin();
	unsigned int glTex;
	while (itr != textures.end()) {
		glTex = itr->second;
		std::cout << "Unregistering texture " << itr->first << std::endl;
		glDeleteTextures(1, &glTex);
		itr++;
	}
	textures.clear();
}

unsigned int TextureManager::Register(FileManager& fm, const std::string& name) {
	if (!fm.fileExists(name)) {
		std::cerr << "Can't load texture " << name << std::endl;
		return(0);
	}

	if (IsRegistered(name))
		return(textures[name]);

	FileData data = fm.getFile(name);
	std::stringstream imgfile;
	data.write(imgfile);
	ImageBMP img(imgfile);

	unsigned int glTex;

	glGenTextures(1, &glTex);
	glBindTexture(GL_TEXTURE_2D, glTex);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // <<- Life saver!!!  
	
	/*
	http://www.opengl.org/sdk/docs/man/xhtml/glTexImage2D.xml
	GL_INVALID_VALUE is generated if non-power-of-two textures are not supported and the width or height cannot be represented as 
	2^k + 2*border for some integer value of k.
	*/

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		img.getBpp() / 8,
		img.getWidth(),
		img.getHeight(),
		0,
		//img.glDrawType,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		img.getBuffer()
	);

	textures[name] = glTex;

	return(glTex);
}

bool TextureManager::UnRegister(const std::string& name) {
	if (!IsRegistered(name))
		return(false);
	tex_t::iterator itr = textures.find(name);

	unsigned int glTex = itr->second;
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

	if (!glIsTexture(itr->second))
		return(false);

	glBindTexture(GL_TEXTURE_2D, itr->second);
	return(true);
}


unsigned int TextureManager::operator [](const std::string& name) const {
	tex_t::const_iterator itr = textures.find(name);
	if (itr == textures.end())
		return(0);
	return(itr->second);
}