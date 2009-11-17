/* $Id$ */
/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of The Open Ragnarok Project
    Copyright 2007 - 2009 The Open Ragnarok Team
    For the latest information visit http://www.open-ragnarok.org
    ------------------------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place - Suite 330, Boston, MA 02111-1307, USA, or go to
    http://www.gnu.org/copyleft/lesser.txt.
    ------------------------------------------------------------------------------------
*/
#include "stdafx.h"
#include "ro/types/spr.h"

#include <fstream>
#include <sstream>

namespace RO {

#pragma pack(push,1)
struct bmpHeader {
	struct fileHeader {
		unsigned short magic;
		unsigned int size;
		unsigned short reserved1;
		unsigned short reserved2;
		unsigned int offset;
	} bmp;
	struct dibHeader {
		unsigned int size; // 40
		int w, h;
		unsigned short planes; // 1
		unsigned short bpp; // 8 in our case
		unsigned int compression;
		unsigned int datasize;
		unsigned int hres, vres;
		unsigned int colorcount;
		unsigned int importantcolors;
	} dib;
};
#pragma pack(pop)


SPR::SPR() : Object() {
	magicSize = 2;
	m_pal = NULL;
}

SPR::~SPR() {
	reset();
}

bool SPR::readStream(std::istream& s) {
	reset();
	if (!readHeader(s)) {
		return(false);
	}
	if (!checkHeader("SP")) {
		_log(ROINT__DEBUG, "Invalid SPR header (%c%c)", magic[0], magic[1]);
		return(false);
	}

	switch (m_version.sver) {
		default:
			_log(ROINT__DEBUG, "Unsupported SPR version (%u.%u)", (m_version.sver >> 8) & 0xFF, m_version.sver & 0xFF);
			return(false);
		case 0x100:
		case 0x101:
		case 0x200:
		case 0x201:
			break;// supported
	}

	unsigned short i;
	unsigned short imgCountPal, imgCountRgba;

	s.read((char*)&imgCountPal, sizeof(unsigned short));
	if (m_version.sver >= 0x200)
		s.read((char*)&imgCountRgba, sizeof(unsigned short));
	else
		imgCountRgba = 0;
	if (s.fail()) {
		return(false);
	}

	if (imgCountPal > 0)
	{
		Image empty = {PalType,0,0,NULL};
		m_imagesPal.resize(imgCountPal, empty);
		for (i = 0; i < imgCountPal; i++) {
			if (!readImagePal(s, i)) {
				reset();
				return(false);
			}
		}
	}
	if (imgCountRgba > 0)
	{
		Image empty = {RgbaType,0,0,NULL};
		m_imagesRgba.resize(imgCountRgba, empty);
		for (i = 0; i < imgCountRgba; i++) {
			if (!readImageRgba(s, i)) {
				reset();
				return(false);
			}
		}
	}

	if (m_version.sver >= 0x101) {
		m_pal = new RO::PAL();
		m_pal->readStream(s);
	}
	if (s.fail()) {
		reset();
		return(false);
	}
	m_valid = true;
	return(true);
}

unsigned int SPR::getImageCount(ImageType type) const {
	switch (type) {
		case PalType:
			return(m_imagesPal.size());
		case RgbaType:
			return(m_imagesRgba.size());
	}
	return(0);
}

unsigned int SPR::getImageCount() const {
	return(m_imagesPal.size() + m_imagesRgba.size());
}

bool SPR::readImagePal(std::istream& s, unsigned int idx) {
	Image& image = m_imagesPal[idx];

	s.read((char*)&image.width, sizeof(unsigned short));
	s.read((char*)&image.height, sizeof(unsigned short));
	if (s.fail()) {
		return(false);
	}
	unsigned int imageSize = (unsigned int)image.width * image.height;

	if (m_version.sver >= 0x201) {
		// rle encoded
		unsigned char c;
		unsigned short encodedSize;

		s.read((char*)&encodedSize, sizeof(unsigned short));
		if (s.fail()) {
			return(false);
		}

		unsigned short pos;
		unsigned char* data = new unsigned char[encodedSize];
		s.read((char*)data, encodedSize);
		if (s.fail()) {
			delete[] data;
			return(false);
		}
		std::stringstream out;
		int written = 0;

		for (pos = 0; pos < encodedSize; pos++) {
			c = data[pos];

			if (c == 0) {
				// rle-encoded (invisible/background palette index)
				pos++;
				if (pos >= encodedSize) {
					_log(ROINT__DEBUG, "invalid encoded data for SPR pal image (idx=%u width=%u height=%u encodedSize=%u)", idx, image.width, image.height, encodedSize);
					delete[] data;
					return(false);
				}
				unsigned char len = data[pos];
				if (len == 0)
					len = 1;
				for (unsigned char i = 0; i < len; i++) {
					out.put('\0');
				}
			}
			else {
				out.put(c);
			}
		}

		delete[] data;

		unsigned int decodedSize = (unsigned int)out.tellp();
		if (decodedSize != imageSize) {
			_log(ROINT__DEBUG, "invalid decoded size for SPR pal image (idx=%u width=%u height=%u decodedSize=%u)", idx, image.width, image.height, decodedSize);
			return(false);
		}
		image.data.pal = new unsigned char[imageSize];
		out.read((char*)image.data.pal, imageSize);
	}
	else {
		// raw
		image.data.pal = new unsigned char[imageSize];
		s.read((char*)image.data.pal, imageSize);
	}
	return(!s.fail());
}

bool SPR::readImageRgba(std::istream& s, unsigned int idx) {
	Image& image = m_imagesRgba[idx];

	s.read((char*)&image.width, sizeof(unsigned short));
	s.read((char*)&image.height, sizeof(unsigned short));
	if (s.fail()) {
		return(false);
	}

	unsigned int length = (unsigned int)image.width * image.height;
	image.data.rgba = new Color[length];
	s.read((char*)image.data.rgba, length * sizeof(Color));
	return(!s.fail());
}

void SPR::reset() {
	unsigned int i;
	m_valid = false;
	for (i = 0; i < m_imagesPal.size(); i++) {
		Image& image = m_imagesPal[i];
		if (image.data.pal != NULL) {
			delete[] image.data.pal;
			image.data.pal = NULL;
		}
	}
	m_imagesPal.clear();
	for (i = 0; i < m_imagesRgba.size(); i++) {
		Image& image = m_imagesRgba[i];
		if (image.data.rgba != NULL) {
			delete[] image.data.rgba;
			image.data.rgba = NULL;
		}
	}
	m_imagesRgba.clear();
	if (m_pal != NULL) {
		delete m_pal;
		m_pal = NULL;
	}
}

const RO::SPR::Image* RO::SPR::getImage(unsigned int idx, ImageType type) const {
	switch (type) {
		case PalType:
			if (idx < m_imagesPal.size())
				return(&m_imagesPal[idx]);
			break;
		case RgbaType:
			if (idx < m_imagesRgba.size())
				return(&m_imagesRgba[idx]);
			break;
	}
	return(NULL);
}

const RO::SPR::Image* RO::SPR::getImage(unsigned int idx) const {
	if (idx < m_imagesPal.size())
		return(&m_imagesPal[idx]);
	idx -= m_imagesPal.size();
	if (idx < m_imagesRgba.size())
		return(&m_imagesRgba[idx]);
	return(NULL);
}

unsigned int RO::SPR::getIndex(unsigned int idx, ImageType type) const {
	switch (type) {
		case PalType:
			if (idx < m_imagesPal.size())
				return(idx);
			break;
		case RgbaType:
			if (idx < m_imagesRgba.size())
				return(m_imagesPal.size() + idx);
			break;
	}
	return((unsigned int)-1);
}

const PAL* RO::SPR::getPal() const {
	return(m_pal);
}

bool SPR::saveBMP(unsigned int idx, std::ostream& s, ImageType type, const RO::PAL* pal) const {
	const Image* img = getImage(idx, type);
	if (img == NULL)
		return(false);

	bmpHeader header;

	memset((char*)&header, 0, sizeof(bmpHeader));
	header.bmp.magic = 0x4D42; // 'BM'
	header.dib.size = 40;
	header.dib.planes = 1;
	header.dib.compression = 0;
	header.dib.vres = 1;
	header.dib.hres = 1;
	header.dib.w = img->width;
	header.dib.h = img->height;

	if (type == PalType) {
		if (pal == NULL)
			pal = m_pal;
		if (pal == NULL)
			return(false);// no palette

		unsigned int k;
		int wscan = img->width;

		while (wscan % 4 != 0)
			wscan++;

		header.dib.bpp = 8;
		header.dib.colorcount = 256;
		header.dib.importantcolors = 0;
		header.dib.datasize = wscan * img->height;
		header.bmp.offset = sizeof(bmpHeader) + 256 * 4;
		header.bmp.size = header.bmp.offset + header.dib.datasize;

		s.write((char*)&header, sizeof(bmpHeader));
		for (k = 0; k < 256; k++) {
			s.write((char*)&pal->getColor(k), 4);
		}

		char* dbuf = new char[wscan];
		memset(dbuf, 0, wscan);

		// left to right, top to bottom -> left to right, bottom to top
		for (int ii = img->height - 1; ii >= 0; ii--) {
			memcpy(dbuf, &img->data.pal[ii * img->width], img->width);
			s.write(dbuf, wscan);
		}

		delete[] dbuf;
	}
	else if (type == RgbaType) {
		unsigned int length = (unsigned int)img->width * img->height;

		header.dib.bpp = 32;
		header.dib.colorcount = 0;
		header.dib.importantcolors = 0;
		header.dib.datasize = 4 * length;
		header.bmp.offset = sizeof(bmpHeader);
		header.bmp.size = header.bmp.offset + header.dib.datasize;

		s.write((char*)&header, sizeof(bmpHeader));

		// left to right, bottom to top
		for (unsigned int i = 0; i < length; i++) {
			// RGBA -> ARGB
			const Color& pixel = img->data.rgba[i];
			s.put(pixel.b);// B
			s.put(pixel.g);// G
			s.put(pixel.r);// R
			s.put(pixel.a);// A
		}
	}
	else {
		_log(ROINT__DEBUG, "invalid SPR image type %d", type);
		return(false);
	}

	return(!s.fail());
}

bool SPR::saveBMP(unsigned int idx, const std::string& fn, ImageType type, const RO::PAL* pal) const {
	std::ofstream fp(fn.c_str(), std::ios_base::binary);
	bool ret = saveBMP(idx, fp, type, pal);
	fp.close();
	return(ret);
}

bool SPR::saveBMP(std::ostream& s, ImageType type, const RO::PAL* pal) const {
	unsigned int imageWidth = 512;
	unsigned int imageHeight = 0;
	unsigned int i;

	// calculate image size
	unsigned int linespace = imageWidth;
	unsigned int lineheight = 0;
	for (i = 0; i < getImageCount(type); i++) {
		const Image* img = getImage(i, type);
		if (imageWidth < img->width) {
			_log(ROINT__DEBUG, "SPR frame too big!");
			return(false);
		}
		if (linespace < img->width) {
			// Too big to fit this line. Increase one line.
			imageHeight += lineheight;
			linespace = imageWidth;
			lineheight = 0;
		}
		if (lineheight < img->height)
			lineheight = img->height;
		linespace -= img->width;
	}
	imageHeight += lineheight;

	bmpHeader header;

	memset((char*)&header, 0, sizeof(bmpHeader));
	header.bmp.magic = 0x4D42; // 'BM'
	header.dib.size = 40;
	header.dib.planes = 1;
	header.dib.compression = 0;
	header.dib.vres = 1;
	header.dib.hres = 1;

	if (type == PalType) {
		// palette images
		if (pal == NULL)
			pal = m_pal;
		if (pal == NULL)
			return(false);// no palette

		while (imageWidth % 4 != 0)
			imageWidth++;

		header.dib.w = imageWidth;
		header.dib.h = imageHeight;
		header.dib.bpp = 8;
		header.dib.colorcount = 256;
		header.dib.importantcolors = 0;
		header.dib.datasize = imageWidth * imageHeight;
		header.bmp.offset = sizeof(bmpHeader) + 256 * 4;
		header.bmp.size = header.bmp.offset + header.dib.datasize;

		s.write((char*)&header, sizeof(bmpHeader));
		for (i = 0; i < 256; i++) {
			s.write((char*)&pal->getColor(i), 4);
		}

		unsigned char* dbuf = new unsigned char[header.dib.datasize];
		memset(dbuf, 0, header.dib.datasize);

		unsigned int offx = 0;
		unsigned int offy = 0;
		lineheight = 0;
		for (i = 0; i < getImageCount(type); i++) {
			const Image* img = getImage(i, type);
			if (imageWidth < offx + img->width) {
				// Too big to fit this line. Increase one line.
				offy += lineheight;
				offx = 0;
				lineheight = 0;
			}
			if (lineheight < img->height)
				lineheight = img->height;
			// left to right, top to bottom -> left to right, bottom to top
			for (unsigned int y = 0; y < img->height; y++) {
				memcpy(&dbuf[offx + (offy + y) * imageWidth], &img->data.pal[(img->height - y - 1) * img->width], img->width);
			}
			offx += img->width;
		}

		s.write((char*)dbuf, header.dib.datasize);
		delete[] dbuf;
	}
	else if (type == RgbaType) {
		// rgba images
		unsigned int length = (unsigned int)imageWidth * imageHeight;

		header.dib.bpp = 32;
		header.dib.colorcount = 0;
		header.dib.importantcolors = 0;
		header.dib.datasize = 4 * length;
		header.bmp.offset = sizeof(bmpHeader);
		header.bmp.size = header.bmp.offset + header.dib.datasize;

		s.write((char*)&header, sizeof(bmpHeader));

		unsigned char* dbuf = new unsigned char[header.dib.datasize];
		memset(dbuf, 0, header.dib.datasize);

		unsigned int offx = 0;
		unsigned int offy = 0;
		lineheight = 0;
		for (i = 0; i < getImageCount(type); i++) {
			const Image* img = getImage(i, type);
			if (imageWidth < offx + img->width) {
				// Too big to fit this line. Increase one line.
				offy += lineheight;
				offx = 0;
				lineheight = 0;
			}
			if (lineheight < img->height)
				lineheight = img->height;
			// left to right, bottom to top
			for (unsigned int x = 0; x < img->width; x++) {
				for (unsigned int y = 0; y < img->height; y++) {
					// RGBA -> ARGB
					const Color& pixel = img->data.rgba[x + y * img->width];
					unsigned char* p = &dbuf[4 * (offx + x + (offy + y) * imageWidth)];
					p[0] = pixel.b;// B
					p[1] = pixel.g;// G
					p[2] = pixel.r;// R
					p[3] = pixel.a;// A
				}
			}
			offx += img->width;
		}

		s.write((char*)dbuf, header.dib.datasize);
		delete[] dbuf;
	}
	else {
		_log(ROINT__DEBUG, "invalid SPR image type %d", type);
		return(false);
	}
	return(!s.fail());
}

}
