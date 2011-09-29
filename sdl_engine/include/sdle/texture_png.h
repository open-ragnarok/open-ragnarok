/* $Id$
 * texture_png.h
 *
 *  Created on: 2009-09-09
 *      Author: Sergio Moura
 *    Revision: $Rev$
 */
#ifndef __SDLENGINE_TEXTURE_PNG_H
#define __SDLENGINE_TEXTURE_PNG_H

#include "texture.h"
#include <png.h>

namespace sdle {
class SDLENGINE_DLLAPI PngLoader {
protected:
	/** Indicates if we are valid or not. Valid = 1, Invalid = 0 */
	bool m_valid;
public:
	PngLoader(const char*);
	~PngLoader();

	bool Valid() const;

	FILE* fp;

	png_structp png_ptr;
	png_infop info_ptr;
	png_uint_32 width, height;
	//int widthpot, heightpot;

};

/** Loads a PNG from a file and creates a Texture */
SDLENGINE_DLLAPI Texture loadPngTexture(const char*);

/**
 * Loads a PNG from a the memory and creates a Texture
 * @param buf
 * @param buflen Length of the buf parameter
 */
SDLENGINE_DLLAPI Texture loadPngTexture(const char* buf, unsigned int buflen);
}

#endif /* __SDLENGINE_TEXTURE_PNG_H */
