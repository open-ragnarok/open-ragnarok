/* $Id$ */
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
#include "stdafx.h"
#include "sdle/image.h"
#include <sstream>

namespace sdle {
Image::Image(char* img, int w, int h, int bpp) : DynamicBlob(img, w*h*(bpp/8)) {
	this->m_bpp = bpp;
	if ((bpp != 24) && (bpp != 32) && (bpp != 8)) {
		// TODO: Fix log warning
		fprintf(stderr, "WARNING: Invalid bpp (%d) when creating a new image\n", bpp);
	}
	m_height = h;
	m_width = w;
#ifdef BMP_DEBUG
	printf("Creating image object with dimensions %dx%d (%lu)\n", m_width, m_height, dataSize);
#endif
}

Image::~Image() {
}

void Image::Dump(std::ostream& out) const {
	out << "=== BITMAP DUMP ===" << std::endl;
	out << "BPP" << m_bpp << std::endl;
	out << "Dimensions: " << m_width << "x" << m_height << std::endl;
	out << "Size: " << blobSize() << std::endl;
	out << "===================" << std::endl;
}

int Image::getWidth() const {
	return(m_width);
}

int Image::getHeight() const {
	return(m_height);
}

int Image::getBpp() const {
	return(m_bpp);
}

bool Image::resizeW(const int& w) {
	if (m_width == w)
		return(true);
	DynamicBlob original(*this);
	int maxw = (w > m_width)?m_width:w;
	setSize(w * m_height * (m_bpp/8));

	int linesize = maxw * (m_bpp/8);

	const char* original_pos = original.getBuffer();
	char* dest_pos = getBuffer();
	for (int i = 0; i < m_height; i++) {
		memcpy(dest_pos, original_pos, linesize);

		original_pos += m_width * (m_bpp/8);
		dest_pos += w * (m_bpp/8);
	}

	m_width = w;
	return(true);
}
bool Image::resizeH(const int& h) {
	if (m_height == h)
		return(true);
	setSize((m_bpp/8) * m_width * h);
	m_height = h;
	return(true);
}
}
