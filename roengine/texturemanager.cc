/* $Id$ */
#include "stdafx.h"

#include "texturemanager.h"

#include "image_bmp.h"

std::list<Texture*> Texture::objects;

Texture::Texture() {
	objects.push_back(this);
	refcount = 0;
	texid = 0;
}

Texture::Texture(const Texture& t) {
	objects.push_back(this);
	refcount = 0;
	texid = t.texid;
}

Texture::Texture(const unsigned int& t, const std::string& n) {
	objects.push_back(this);
	refcount = 0;
	texid = t;
	name = n;
}

Texture::~Texture() {
	if (refcount != 0)
		std::cerr << "Warning: Deleting texture " << name << " with "<< refcount<<" active references!" << std::endl;

	objects.remove(this);

	glDeleteTextures(1, &texid);
}

void Texture::AddRef() {
	refcount++;
}

void Texture::Release() {
	refcount--;
}

Texture& Texture::operator = (const Texture& t) {
	texid = t.texid;
	return(*this);
}

Texture& Texture::operator = (unsigned int i) {
	texid = i;
	return(*this);
}

unsigned int Texture::operator*() const {
	return(texid);
}

unsigned int Texture::getIdx() const {
	return(texid);
}

void Texture::Activate() const {
	glBindTexture(GL_TEXTURE_2D, texid);
}

// === TEXTURE POINTER ===

Texture::Pointer::Pointer() {
	texture = NULL;
}

Texture::Pointer::Pointer(Texture* t) {
	texture = t;
	if (texture != NULL)
		texture->AddRef();
}

Texture::Pointer::Pointer(Pointer& t) {
	texture = t.texture;
	if (texture != NULL)
		texture->AddRef();
}

Texture::Pointer::~Pointer() {
	if (texture != NULL)
		texture->Release();
}

void Texture::Pointer::setTexture(Texture* t) {
	if (texture != NULL)
		texture->Release();
	texture = t;
	if (texture != NULL)
		texture->AddRef();
}

Texture::Pointer& Texture::Pointer::operator = (Texture *t) {
	if (texture != NULL)
		texture->Release();
	texture = t;
	if (texture != NULL)
		texture->AddRef();

	return(*this);
}

Texture::Pointer& Texture::Pointer::operator = (Pointer& t) {
	if (texture != NULL)
		texture->Release();
	texture = t.texture;
	if (texture != NULL)
		texture->AddRef();
	return(*this);
}

unsigned int Texture::Pointer::operator *() const {
	if (texture == NULL)
		return(0);
	return(texture->getIdx());
}

void Texture::Pointer::Activate() const {
	if (texture != NULL)
		texture->Activate();
}

Texture::PointerCache::PointerCache() {}

Texture::PointerCache::~PointerCache() {
	clear();
}

void Texture::PointerCache::clear() {
	std::vector<Pointer*>::iterator itr;
	itr = pointers.begin();
	while(itr != pointers.end()) {
		delete(*itr);
		itr++;
	}
	pointers.clear();
}

Texture::Pointer* Texture::PointerCache::operator[] (const unsigned int& i) {
	return(pointers[i]);
}

const Texture::Pointer* Texture::PointerCache::operator[] (const unsigned int& i) const {
	return(pointers[i]);
}

void Texture::PointerCache::add(Texture::Pointer* p) {
	pointers.push_back(p);
}

// === TEXTURE MANAGER ===

TextureManager::TextureManager() {
}

TextureManager::~TextureManager() {
	Clear();
}

void TextureManager::Clear() {
	tex_t::iterator itr = textures.begin();
	Texture* tp;

	unsigned int glTex;
	while (itr != textures.end()) {
		tp = itr->second;
		glTex = tp->getIdx();
		// std::cout << "Unregistering texture " << itr->first << std::endl;
		delete(tp);
		itr++;
	}
	textures.clear();
}

Texture::Pointer TextureManager::Register(FileManager& fm, const std::string& name) {
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

	textures[name] = new Texture(glTex, name);
	Texture::Pointer tp(textures[name]);

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


Texture::Pointer TextureManager::operator [](const std::string& name) const {
	tex_t::const_iterator itr = textures.find(name);
	if (itr == textures.end())
		return(0);
	Texture::Pointer ptr(itr->second);
	return(ptr);
}