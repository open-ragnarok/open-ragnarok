/* $id$ */
/*
 Open Ragnarok Project 

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2 of the License.
  
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY except by those people which sell it, which
 are required to give you total support for your newly bought product;
 without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
	
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "image.h"
#include <sstream>

Image::Image(char* img, int w, int h, int bpp) : DynamicBlob(w*h*(bpp/8)) {
	if (img)
		memcpy(buffer, img, dataSize);
	this->bpp = bpp;
	if ((bpp != 24) && (bpp != 32)) {
		// TODO: Fix log warning
		fprintf(stderr, "WARNING: Invalid bpp (%d) when creating a new image\n", bpp);
	}
	height = h;
	width = w;
#ifdef BMP_DEBUG
	printf("Creating image object with dimensions %dx%d (%lu)\n", width, height, dataSize);
#endif
}

Image::~Image() {
}

int Image::getWidth() const {
	return(width);
}

int Image::getHeight() const {
	return(height);
}

int Image::getBpp() const {
	return(bpp);
}
