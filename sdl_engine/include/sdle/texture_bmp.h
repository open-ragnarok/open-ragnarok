/* $Id$
 * texture_bmp.h
 *
 *  Created on: 2009-09-09
 *      Author: Sergio Moura
 *    Revision: $Rev$
 */
#ifndef TEXTURE_BMP_H_
#define TEXTURE_BMP_H_

#include "texture.h"

namespace sdle {
/** Loads a Bitmap texture from a file */
SDLENGINE_DLLAPI Texture loadBMPTexture(const char*);
}

#endif /* TEXTURE_BMP_H_ */
