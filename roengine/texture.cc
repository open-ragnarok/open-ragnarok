#include "stdafx.h"

#include "texture.h"

#include "sdl_engine.h"

std::list<Texture*> Texture::objects;

Texture::Texture() {
	objects.push_back(this);
	refcount = 0;
	texid = 0;
	m_width = 0;
	m_height = 0;
	m_maxu = 1.0f;
	m_maxv = 1.0f;
}

int minPOT(const int& k) {
	int n = 2;
	while (n < k)
		n <<= 1;

	return(n);
}

Texture::Texture(const Image& img, const std::string& n) {
	objects.push_back(this);
	name = n;

	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);

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

	m_width = img.getWidth();
	m_height = img.getHeight();
	unsigned int drawtype = GL_RGB;
	if (img.getBpp() == 32)
		drawtype = GL_RGBA;

	Image imgpot(img);

	if (!SDLEngine::supportPot) {
		int k;
		if ((k = minPOT(m_width)) != m_width) {
			imgpot.resizeW(k);
			m_maxu = (float)m_width / (float)k;
		}
		if ((k = minPOT(m_height)) != m_height) {
			imgpot.resizeH(k);
			m_maxv = (float)m_height / (float)k;
		}
	}

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		imgpot.getBpp() / 8,
		imgpot.getWidth(),
		imgpot.getHeight(),
		0,
		drawtype,
		GL_UNSIGNED_BYTE,
		imgpot.getBuffer()
	);

	int r = glGetError();
	if (r != 0) {
		printf("error registering texture %s: ", name.c_str());
		switch(r) {
			case GL_INVALID_ENUM:
				printf("GL_INVALID_ENUM");
				break;
			case GL_INVALID_VALUE:
				printf("GL_INVALID_VALUE");
				break;
			case GL_INVALID_OPERATION:
				printf("GL_INVALID_OPERATION");
				break;
			default:
				printf("%d", r);
		}

		printf(" size: %d x %d", m_width, m_height);
		printf("\n");
	}

	refcount = 0;
}

Texture::Texture(const Texture& t) {
	objects.push_back(this);
	refcount = 0;
	texid = t.texid;
	m_width = t.m_width;
	m_height = t.m_height;
	name = t.name;
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

int Texture::getWidth() const {
	return(m_width);
}

int Texture::getHeight() const {
	return(m_height);
}

Texture& Texture::operator = (const Texture& t) {
	texid = t.texid;
	name = t.name;
	m_width = t.m_width;
	m_height = t.m_height;
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
	if (!glIsTexture(texid))
		std::cerr << "Error activating invalid texture " << name << std::endl;
	glBindTexture(GL_TEXTURE_2D, texid);
}

float Texture::getMaxU() const {
	return(m_maxu);
}

float Texture::getMaxV() const {
	return(m_maxv);
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

Texture::Pointer::Pointer(const Pointer& t) {
	texture = NULL;
	*this = t;
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

Texture::Pointer& Texture::Pointer::operator = (const Pointer& t) {
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

const Texture* Texture::Pointer::operator ->() const {
	return(texture);
}


void Texture::Pointer::Activate() const {
	if (texture != NULL)
		texture->Activate();
}

bool Texture::Pointer::isValid() const {
	return(texture != NULL);
}

Texture::PointerCache::PointerCache() {}

Texture::PointerCache::PointerCache(const PointerCache& pc) {
	std::vector<Pointer>::const_iterator itr = pc.pointers.begin();
	while (itr != pc.pointers.end()) {
		pointers.push_back(*itr);
		itr++;
	}
}

Texture::PointerCache::~PointerCache() {
	clear();
}

void Texture::PointerCache::clear() {
	pointers.clear();
}

unsigned int Texture::PointerCache::size() const {
	return(pointers.size());
}

Texture::Pointer& Texture::PointerCache::operator[] (const unsigned int& i) {
	return(pointers[i]);
}

const Texture::Pointer& Texture::PointerCache::operator[] (const unsigned int& i) const {
	return(pointers[i]);
}

void Texture::PointerCache::add(const Texture::Pointer& p) {
	Texture::Pointer pointer(p);
	pointers.push_back(pointer);
}

Texture::PointerCache& Texture::PointerCache::operator = (const PointerCache& pc) {
	clear();
	for (unsigned int i = 0; i < pc.size(); i++) {
		pointers.push_back(pc[i]);
	}
	return(*this);
}
