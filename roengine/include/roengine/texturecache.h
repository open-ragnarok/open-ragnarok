#ifndef __ROENGINE_TEXTURECACHE_H
#define __ROENGINE_TEXTURECACHE_H

#include "sdle/texture.h"
#include <vector>

class TextureCache {
protected:
	std::vector<sdle::Texture> m_textures;
public:
	TextureCache();
	~TextureCache();

	void add(const sdle::Texture&);
	void clear();

	sdle::Texture& get(unsigned int i);
	const sdle::Texture& get(unsigned int i) const;

	sdle::Texture& operator[](unsigned int i);
	const sdle::Texture& operator[](unsigned int i) const;
};

#endif /* __ROENGINE_TEXTURECACHE_H */
