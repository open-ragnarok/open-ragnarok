/* $Id: image_spr.cc 45 2009-02-12 13:24:41Z sergio $ */
#include "stdafx.h"

#include "rogl/image_spr.h"

rogl::ImageSPR::ImageSPR(const RO::SPR* s, const unsigned int& idx) : Image(NULL, 0, 0) {
	const RO::SPR::Image* img = s->getFrame(idx);
	
	if (img != NULL) {
		width = img->w;
		height = img->h;
		bpp = 32;
		setSize(width * height * 4);
		const RO::SPR::Pal* p;
		int pos;
		
		//printf("reading sprite %u x %u\n", width, height);
		for(int x = 0; x < width; x++) {
			for(int y = 0; y < height; y++) {
				pos = y * width + x;
				unsigned char ppos = img->data[pos];
				p = s->getPal(ppos);
				if (ppos != 0) {
					buffer[pos * 4] = p->r;
					buffer[pos * 4 + 1] = p->g;
					buffer[pos * 4 + 2] = p->b;
					buffer[pos * 4 + 3] = 255 - p->a;
				}
				else {
					buffer[pos * 4] = 0;
					buffer[pos * 4 + 1] = 255;
					buffer[pos * 4 + 2] = 0;
					buffer[pos * 4 + 3] = 0;
				}
			}
		}
	}
}
