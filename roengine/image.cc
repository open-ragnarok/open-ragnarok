/* $Id$ */
#include "stdafx.h"

#include "image.h"
#include <GL/gl.h>

Image::Image(const img_t* img, int w, int h, int bpp) : DynamicBlob<img_t>(w*h*(bpp/8)) {
	if (img)
		memcpy(buffer, img, dataSize);
	this->bpp = bpp;
	if ((bpp != 24) && (bpp != 32)) {
		// TODO: Fix log warning
		fprintf(stderr, "WARNING: Invalid bpp (%d) when creating a new image\n", bpp);
	}
	height = h;
	width = w;
	glDrawType = GL_RGB;
}

Image::~Image() {
}

int Image::getWidth() const { return(width); }
int Image::getHeight() const { return(height); }
int Image::getBpp() const { return(bpp); }
