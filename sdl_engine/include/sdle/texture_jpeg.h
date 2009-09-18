#ifndef __SDLENGINE_TEXTURE_JPEG_H
#define __SDLENGINE_TEXTURE_JPEG_H

#include "texture.h"

namespace sdle {

/** Loads a JPEG texture from a file
 * @param fn the name of the file to load the texture from
 */
SDLENGINE_DLLAPI Texture loadJPEGTexture(const char* fn);

/** Loads a JPEG texture from the memory
 * @param buf the pointer to where the data is stored in the memory
 * @param buflen the size of the provided buffer
 */
SDLENGINE_DLLAPI Texture loadJPEGTexture(const unsigned char* buf, unsigned long buflen);

}

#endif /* __SDLENGINE_TEXTURE_JPEG_H */
