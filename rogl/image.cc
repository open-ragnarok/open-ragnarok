/* $Id$ */
#include "stdafx.h"

#include "rogl/image.h"
#include <GL/gl.h>

rogl::Image::Image(const img_t* img, int w, int h, int bpp) : DynamicBlobUChar(w*h*(bpp/8)) {
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

rogl::Image::Image(const Image& img) : DynamicBlobUChar(img) {
	bpp = img.bpp;
	height = img.height;
	width = img.width;
	glDrawType = img.glDrawType;
}

rogl::Image::~Image() {
}

bool rogl::Image::resizeW(const int& w) {
	DynamicBlob original(*this);
	int maxw = (w > width)?width:w;
	setSize(w * height * (bpp/8));

	int linesize = maxw * (bpp/8) * sizeof(img_t);

	const img_t* original_pos = original.getBuffer();
	img_t* dest_pos = getBuffer();
	for (int i = 0; i < height; i++) {
		memcpy(dest_pos, original_pos, linesize);

		original_pos += width * (bpp/8) * sizeof(img_t);
		dest_pos += w * (bpp/8) * sizeof(img_t);
	}

	width = w;
	return(true);
}
bool rogl::Image::resizeH(const int& h) {
	setSize((bpp/8) * width * h);
	height = h;
	return(true);
}

int rogl::Image::getWidth() const { return(width); }
int rogl::Image::getHeight() const { return(height); }
int rogl::Image::getBpp() const { return(bpp); }
