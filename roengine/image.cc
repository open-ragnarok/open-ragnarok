/* $Id$ */
#include "stdafx.h"

#include "image.h"
#include <GL/gl.h>

Image::Image(const img_t* img, int w, int h, int bpp) : DynamicBlob<img_t>(w*h*(bpp/8)) {
	if (img)
		memcpy(buffer, img, dataSize);
	this->bpp = bpp;
	if ((bpp != 24) && (bpp != 32)) {
		fprintf(stderr, "WARNING: Invalid bpp (%d) when creating a new image\n", bpp);
	}
	height = h;
	width = w;
	glDrawType = GL_RGB;
}

Image::Image(const Image& img) : DynamicBlob<img_t>(img) {
	bpp = img.bpp;
	height = img.height;
	width = img.width;
	glDrawType = img.glDrawType;
}

Image::~Image() {
}

bool Image::resizeW(const int& w) {
	DynamicBlob<img_t> original(*this);
	int maxw = (w > width)?width:w;
	setSize(w * height * (bpp/8));

	const img_t* original_pos = original.getBuffer();
	img_t* dest_pos = getBuffer();
	for (int i = 0; i < height; i++) {
		memcpy(dest_pos, original_pos, maxw * (bpp/8) * sizeof(img_t));

		original_pos += width * (bpp/8) * sizeof(img_t);
		dest_pos += w * (bpp/8) * sizeof(img_t);
	}

	width = w;
	return(true);
}
bool Image::resizeH(const int& h) {
	setSize((bpp/8) * width * h);
	height = h;
	return(true);
}


int Image::getWidth() const { return(width); }
int Image::getHeight() const { return(height); }
int Image::getBpp() const { return(bpp); }
