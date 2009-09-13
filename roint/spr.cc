/* $Id$ */
#include "stdafx.h"
#include "ro/types/spr.h"

#include <fstream>
#include <sstream>

RO::SPR::SPR() : Object() {
	magicSize = 2;
	m_imagesPal = NULL;
	m_imagesRgba = NULL;
	m_pal = NULL;
}

RO::SPR::~SPR() {
	reset();
}

bool RO::SPR::readStream(std::istream& s) {
	reset();
	readHeader(s);
	if (!checkHeader("SP"))
		return(false);

	switch (m_version.sver) {
		default:
			return(false);
		case 0x100:
		case 0x101:
		case 0x200:
		case 0x201:
			break;// supported
	}

	unsigned short i;

	s.read((char*)&m_imgCountPal, sizeof(unsigned short));
	if (m_version.sver >= 0x200)
		s.read((char*)&m_imgCountRgba, sizeof(unsigned short));
	else
		m_imgCountRgba = 0;

	if (m_imgCountPal > 0)
	{
		m_imagesPal = new Image[m_imgCountPal];
		for (i = 0; i < m_imgCountPal; i++)
			readImagePal(s, i);
	}
	if (m_imgCountRgba > 0)
	{
		m_imagesRgba = new Image[m_imgCountRgba];
		for (i = 0; i < m_imgCountRgba; i++)
			readImageRgba(s, i);
	}

	if (m_version.sver >= 0x101) {
		m_pal = new RO::PAL();
		m_pal->readStream(s);
	}
	if (s.eof()) {
		fprintf(stderr, "ERROR: Unexpected end of stream\n");
		reset();
		return(false);
	}

	return(true);
}

unsigned short RO::SPR::getImgCount(const ImageType type) const {
	switch (type) {
		case IT_PAL:
			return(m_imgCountPal);
		case IT_RGBA:
			return(m_imgCountRgba);
	}
	return(0);
}

void RO::SPR::readImagePal(std::istream& s, const unsigned short idx) {
	Image* img = &m_imagesPal[idx];

	char *dPtr;
	dPtr = (char*)&img->w;
	s.read(dPtr, 2);
	dPtr = (char*)&img->h;
	s.read(dPtr, 2);
	if (s.eof()) {
		img->w = 0;
		img->h = 0;
		img->data = NULL;
		return;
	}

	if (m_version.sver >= 0x201) {
		// rle encoded
		unsigned char c;
		unsigned short length;

		s.read((char*)&length, 2);
		if (s.eof()) {
			img->w = 0;
			img->h = 0;
			img->data = NULL;
			return;
		}
		//printf("%u x %u (size: %u)\n", img->w, img->h, length);

		unsigned short pos;
		unsigned char* data = new unsigned char[length];
		s.read((char*)data, length);
		if (s.eof()) {
			delete[] data;
			img->w = 0;
			img->h = 0;
			img->data = NULL;
			return;
		}
		std::stringstream out;
		int written = 0;

		for (pos = 0; pos < length; pos++) {
			c = data[pos];

			if (c == 0) {
				unsigned char len;
				len = data[pos+1];
				pos++;
				if (len != 0) {
					//pos++;
					for (unsigned char j = 0; j < len; j++) {
						out.write("\0", 1);
						written++;
					}
				}
			}
			else {
				out.write((char*)&c, 1);
//				out.write((char*)data + pos, 1);
				written++;
			}
		}

		delete[] data;

		unsigned int size = (unsigned int)out.tellp();
		img->data = new unsigned char[size];
		//printf(" bytes decoded: %u", size);
		//printf(" expected size: %u", img->w * img->h);
		//printf("\n");
		out.read((char*)img->data, size);
	}
	else {
		// raw
		unsigned int length = (unsigned int)img->w * img->h;
		img->data = new unsigned char[length];
		s.read((char*)img->data, length);
		if (s.eof()) {
			delete[] img->data;
			img->w = 0;
			img->h = 0;
			img->data = NULL;
		}
	}
}

void RO::SPR::readImageRgba(std::istream& s, const unsigned short idx) {
	Image* img = &m_imagesRgba[idx];

	char *dPtr;
	dPtr = (char*)&img->w;
	s.read(dPtr, 2);
	dPtr = (char*)&img->h;
	s.read(dPtr, 2);
	if (s.eof()) {
		img->w = 0;
		img->h = 0;
		img->data = NULL;
		return;
	}

	unsigned int length = (unsigned int)4 * img->w * img->h;
	img->data = new unsigned char[length];
	s.read((char*)img->data, length);
	if (s.eof()) {
		delete[] img->data;
		img->w = 0;
		img->h = 0;
		img->data = NULL;
	}
}

void RO::SPR::reset() {
	if (m_imagesPal != NULL) {
		for (unsigned short i = 0; i < m_imgCountPal; i++)
			if (m_imagesPal[i].data != NULL)
				delete[] m_imagesPal[i].data;

		delete[] m_imagesPal;
		m_imagesPal = NULL;
	}
	m_imgCountPal = 0;
	if (m_imagesRgba != NULL) {
		for (unsigned short i = 0; i < m_imgCountRgba; i++)
			if (m_imagesRgba[i].data != NULL )
				delete[] m_imagesRgba[i].data;

		delete[] m_imagesRgba;
		m_imagesRgba = NULL;
	}
	m_imgCountRgba = 0;
	if (m_pal != NULL) {
		delete m_pal;
		m_pal = NULL;
	}
}

const RO::SPR::Image* RO::SPR::getImage(const unsigned short idx, const ImageType type) const {
	switch (type) {
		case IT_PAL:
			if (idx < m_imgCountPal)
				return(&m_imagesPal[idx]);
		case IT_RGBA:
			if (idx < m_imgCountRgba)
				return(&m_imagesRgba[idx]);
	}
	return(NULL);
}

const RO::PAL* RO::SPR::getPal() const {
	return(m_pal);
}

#ifdef MSVC
#	pragma pack(push)
#	pragma pack(1)
#endif
namespace RO {
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
}
#ifdef MSVC
#	pragma pack(pop)
#endif

bool RO::SPR::saveBMP(const unsigned short idx, const ImageType type, std::ostream& s, const RO::PAL* pal) const {
	const Image* img = getImage(idx, type);
	if (img == NULL)
		return(false);
	if (pal == NULL)
		pal = m_pal;

	bmpHeader header;

	memset((char*)&header, 0, sizeof(bmpHeader));
	header.bmp.magic = 0x4D42; // 'BM'
	header.dib.size = 40;
	header.dib.planes = 1;
	header.dib.compression = 0;
	header.dib.vres = 1;
	header.dib.hres = 1;
	header.dib.w = img->w;
	header.dib.h = img->h;

	if (type == IT_PAL) {
		if (pal == NULL)
			return(false);// no palette

		unsigned int k;
		int wscan = img->w;

		while (wscan % 4 != 0)
			wscan++;

		header.dib.bpp = 8;
		header.dib.colorcount = 256;
		header.dib.importantcolors = 0;
		header.dib.datasize = wscan * img->h;
		header.bmp.offset = sizeof(bmpHeader) + 256 * 4;
		header.bmp.size = header.bmp.offset + header.dib.datasize;

		s.write((char*)&header, sizeof(bmpHeader));
		for (k = 0; k < 256; k++) {
			s.write((char*)pal->getPal(k), 4);
		}

		char* dbuf = new char[wscan];
		memset(dbuf, 0, wscan);

		// left to right, top to bottom -> left to right, bottom to top
		for (int ii = img->h - 1; ii >= 0; ii--) {
			memcpy(dbuf, &img->data[ii * img->w], img->w);
			s.write(dbuf, wscan);
		}

		delete[] dbuf;
	}
	else {
		unsigned int length = (unsigned int)img->w * img->h;

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
			char* dPtr = (char*)&img->data[4 * i];
			s.put(dPtr[1]);// B
			s.put(dPtr[2]);// G
			s.put(dPtr[3]);// R
			s.put(dPtr[0]);// A
		}
	}

	return(!s.fail());
}


bool RO::SPR::saveBMP(const unsigned short idx, const ImageType type, const std::string& fn, const RO::PAL* pal) const {
	std::ofstream fp(fn.c_str(), std::ios_base::binary);
	bool ret = saveBMP(idx, type, fp, pal);
	fp.close();
	return(ret);
}

