/* $Id: texture_tga.h -1 $
 * texture_tga.h
 *
 */
#ifndef __SDLENGINE_TEXTURE_TGA_H
#define __SDLENGINE_TEXTURE_TGA_H

#include "texture.h"

namespace sdle {
/** Loads a Targa texture from a file */
SDLENGINE_DLLAPI Texture loadTGATexture(const unsigned char* buf, unsigned long buflen);
}

#endif /* __SDLENGINE_TEXTURE_TGA_H */
