/*
 * texture.cpp
 *
 *  Created on: 14/07/2009
 *      Author: N0C2
 */
#include "stdafx.h"
#include "sdle/texture.h"
#include "sdle/sdl_engine.h"
#include "sdle/image_bmp.h"

namespace sdle {
int minPOT(int k) {
	int ret = 2;
	while(ret < k)
		ret <<=1;
	return(ret);
}

Texture::Texture() {
	m_root = NULL;
}

Texture::Texture(const Texture& t) {
	m_root = t.m_root;
	if (m_root != NULL) {
		m_root->AddRef();
	}
}

Texture::Texture(Root* r) {
	m_root = r;
	if (m_root != NULL) {
		m_root->AddRef();
	}
}

Texture::~Texture() {
	if (m_root != NULL) {
		m_root->DelRef();
		if (m_root->RefCount() == 0) {
			delete (m_root);
		}
	}
}

Texture& Texture::operator =(const Texture& t) {
	if (m_root != NULL) {
		m_root->DelRef();
		if (m_root->RefCount() == 0) {
			delete (m_root);
		}
	}

	m_root = t.m_root;

	if (m_root != NULL) {
		m_root->AddRef();
	}
	return (*this);
}

Texture& Texture::operator =(Root* root) {
	if (m_root != NULL) {
		m_root->DelRef();
		if (m_root->RefCount() == 0) {
			delete (m_root);
		}
	}

	m_root = root;

	if (m_root != NULL) {
		m_root->AddRef();
	}
	return (*this);
}

float Texture::getMaxU() const {
	if (m_root != NULL)
		return (m_root->m_maxu);
	return (0.0f);
}

float Texture::getMaxV() const {
	if (m_root != NULL)
		return (m_root->m_maxv);
	return (0.0f);
}

int Texture::getWidth() const {
	if (m_root != NULL)
		return (m_root->m_width);
	return (0);
}

int Texture::getHeight() const {
	if (m_root != NULL)
		return (m_root->m_height);
	return (0);
}

Texture::Root::Root() {
	glGenTextures(1, &m_texid);
	m_refcount = 0;
}

Texture::Root::~Root() {
	glDeleteTextures(1, &m_texid);
	if (m_refcount != 0) {
		fprintf(stderr, "[WARNING] Deleting texture root with references!");
	}
}

void Texture::Root::AddRef() {
	m_refcount++;
}

void Texture::Root::DelRef() {
	m_refcount--;
}

unsigned int Texture::Root::ID() const {
	return (m_texid);
}

int Texture::Root::RefCount() const {
	return (m_refcount);
}

bool Texture::Root::Valid() const {
	if (glIsTexture(m_texid))
		return (true);
	return(false);
}

bool Texture::Valid() const {
	if (m_root == NULL)
		return (false);

	return (m_root->Valid());
}

bool Texture::Activate() const {
	if (!Valid()) {
		fprintf(stderr, "Error activating invalid texture\n");
		return(false);
	}

	glBindTexture(GL_TEXTURE_2D, m_root->ID());
	return(true);
}

bool Texture::Root::Create(unsigned int texid, int width, int height) {
	if (glIsTexture(texid) == GL_FALSE) {
		return(false);
	}
	m_texid = texid;
	m_width = width;
	m_height = height;
	m_maxu = 1.0f;
	m_maxv = 1.0f;
	return(true);
}


bool Texture::Root::Create(const Image* img) {
	return(Create((void*)img->getBuffer(), img->getWidth(), img->getHeight(), img->getBpp()));
}

bool Texture::Root::Create(void* data, int w, int h, int bpp) {
	if (Valid())
		glDeleteTextures(1, &m_texid);

	m_width = w;
	m_height = h;

	m_maxu = 1.0f;
	m_maxv = 1.0f;

	m_bpp = bpp;

	void* thedata = data;
	int wpot = w;
	int hpot = h;

	Image imgpot((char*)data, w, h, bpp);

	if (!SDLEngine::supportPot()) {
		int k;
		if ((k = minPOT(m_width)) != m_width) {
			imgpot.resizeW(k);
			m_maxu = (float)m_width / (float)k;
		}
		if ((k = minPOT(m_height)) != m_height) {
			imgpot.resizeH(k);
			m_maxv = (float)m_height / (float)k;
		}

		thedata = imgpot.getBuffer();
		wpot = imgpot.getWidth();
		hpot = imgpot.getHeight();
	}

	glGenTextures(1, &m_texid);
	glBindTexture(GL_TEXTURE_2D, m_texid);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // <<- Life saver!!!

	int Bpp = bpp/8;
	int draw = GL_RGB;
	if (bpp == 8) {
		Bpp = GL_ALPHA8;
		draw = GL_ALPHA;
	}
	if (bpp == 32)
		draw = GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, Bpp, wpot, hpot, 0, draw, GL_UNSIGNED_BYTE,
			thedata);

	int r = glGetError();
	if (r != 0) {
		printf("error registering texture: ");
		switch (r) {
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
			printf("GL Error ID: %d", r);
		}

		printf(" size: %d x %d", w, h);
		printf("\n");

		glDeleteTextures(1, &m_texid);
		m_texid = 0;
		return (false);
	} else {
#ifdef DEBUG
		printf("Texture registered (%d x %d)\n", m_width, m_height);
#endif
	}
	return (true);
}
}
