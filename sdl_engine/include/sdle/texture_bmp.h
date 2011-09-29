/* $Id$
 * texture_bmp.h
 *
 *  Created on: 2009-09-09
 *      Author: Sergio Moura
 *    Revision: $Rev$
 */
#ifndef __SDLENGINE_TEXTURE_BMP_H
#define __SDLENGINE_TEXTURE_BMP_H

#include "texture.h"

namespace sdle {
/** Loads a Bitmap texture from a file */
SDLENGINE_DLLAPI Texture loadBMPTexture(const char*);
}

#endif /* __SDLENGINE_TEXTURE_BMP_H */
