/* $Id$
 * texture_png.h
 *
 *  Created on: 2009-09-09
 *      Author: Sergio Moura
 *    Revision: $Rev$
 */
#ifndef TEXTURE_PNG_H_
#define TEXTURE_PNG_H_

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

SDLENGINE_DLLAPI Texture loadPngTexture(const char*);
}

#endif /* TEXTURE_PNG_H_ */
