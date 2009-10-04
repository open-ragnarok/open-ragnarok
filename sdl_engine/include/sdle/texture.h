/*
 * texture.h
 *
 *  Created on: 2009-07-14
 *      Author: Sergio Moura
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "sdle_import.h"
#include "image.h"

namespace sdle {
/**
 * Describes a texture object, being able to copy to other places
 * and keeps only one instance of the GL object in the memory.
 */
class SDLENGINE_DLLAPI Texture {
public:
	/** The Root object containing the actual texture information for drawing.
	 *
	 * Note that when this object is created, it checks for npot texture support.
	 * If it does not finds the support, the texture is automatically resized to
	 * the next pot size and maxu and maxv are set accordingly. This can consume
	 * quite some time when loading many textures.
	 */
	class SDLENGINE_DLLAPI Root {
	protected:
		/** The Texture id reference */
		unsigned int m_texid;

		/** How many references to this texture we have */
		int m_refcount;

		/** How many bpps this texture has */
		unsigned int m_bpp;

	public:
		Root();
		~Root();

		/** Creates a texture from raw memory data */
		bool Create(void* data, int w, int h, int bpp = 32);

		/** Creates a texture from an Image object */
		bool Create(const Image*);

		/** 
		 * Creates a texture from an already created id and given properties.
		 * maxu and maxv will be set to 1.0f
		 * texid will be check for validity
		 */
		bool Create(unsigned int texid, int width, int height);

		int m_width, m_height;
		float m_maxu, m_maxv;

		/** Adds a reference to our object */
		void AddRef();

		/** Deletes a reference to our object. Once we have no references, this object is destroyed. */
		void DelRef();

		/** Retrieves the texture ID value */
		unsigned int ID() const;

		/** Retrieves the number of references we have */
		int RefCount() const;

		/** Checks if the texture in this object is valid */
		bool Valid() const;
	};

protected:
	Root* m_root;

public:
	Texture();
	Texture(const Texture&);
	Texture(Root*);
	virtual ~Texture();

	int getWidth() const;
	int getHeight() const;

	float getMaxU() const;
	float getMaxV() const;

	Texture& operator = (const Texture&);
	Texture& operator = (Root*);
	bool operator == (const Texture&) const;

	bool Activate() const;
	bool Valid() const;
};
}

#endif /* TEXTURE_H_ */
