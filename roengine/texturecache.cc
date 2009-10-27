#include "stdafx.h"

#include "roengine/texturecache.h"

TextureCache::TextureCache() {}
TextureCache::~TextureCache() {}

void TextureCache::add(const sdle::Texture& t) {
	m_textures.push_back(t);
}

void TextureCache::clear() {
	m_textures.clear();
}

sdle::Texture& TextureCache::get(unsigned int i) {
	return(m_textures[i]);
}

const sdle::Texture& TextureCache::get(unsigned int i) const {
	return(m_textures[i]);
}

sdle::Texture& TextureCache::operator[](unsigned int i) {
	return(m_textures[i]);
}

const sdle::Texture& TextureCache::operator[](unsigned int i) const {
	return(m_textures[i]);
}
