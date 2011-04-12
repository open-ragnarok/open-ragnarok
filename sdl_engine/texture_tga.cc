/* $Id: texture_tga.cc -1 $
 * texture_tga.cc
 *
 */
#include "stdafx.h"
#include "sdle/texture_tga.h"
//#include "sdle/image_tga.h"
#include "libtarga.h"

namespace sdle {
/** Loads a Targa texture from a file */
Texture loadTGATexture(const unsigned char* buf, unsigned long buflen) {
	Texture::Root* root;
//	ImageTGA image(fn);
	unsigned char *data;
	int width, height;
	data = (unsigned char*)tga_load_mem(buf, buflen, &width, &height, TGA_TRUECOLOR_32);

	root = new Texture::Root();

//	if (!root->Create(&image)) {
	if (!root->Create(data, width, height)) {
		delete(root);
		return(Texture());
	}

	return(Texture(root));
}
}
