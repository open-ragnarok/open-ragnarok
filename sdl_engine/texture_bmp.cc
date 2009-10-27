/* $Id$
 * texture_bmp.cc
 *
 *  Created on: 2009-09-09
 *      Author: Sergio Moura
 *    Revision: $Rev$
 */
#include "stdafx.h"
#include "sdle/texture_bmp.h"
#include "sdle/image_bmp.h"

namespace sdle {
/** Loads a Bitmap texture from a file */
Texture loadBMPTexture(const char* fn) {
	Texture::Root* root;
	ImageBMP image(fn);

	root = new Texture::Root();

	if (!root->Create(&image)) {
		delete(root);
		return(Texture());
	}

	return(Texture(root));
}
}
