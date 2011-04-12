/* $Id: texture_tga.h -1 $
 * texture_tga.h
 *
 */
#ifndef TEXTURE_TGA_H_
#define TEXTURE_TGA_H_

#include "texture.h"

namespace sdle {
/** Loads a Targa texture from a file */
SDLENGINE_DLLAPI Texture loadTGATexture(const unsigned char* buf, unsigned long buflen);
}

#endif /* TEXTURE_TGA_H_ */
