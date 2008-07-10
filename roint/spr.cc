/* $id$ */
#include "stdafx.h"
#include "spr.h"

#include <fstream>
#include <sstream>

RO::SPR::SPR() : Object() {
	magicSize = 2;
	images = NULL;
}

RO::SPR::~SPR() {
	if (images != NULL) {
		for (unsigned int i = 0; i < imgCount; i++)
			delete[] images[i].data;

		delete[] images;
		images = NULL;
	}
}

bool RO::SPR::readStream(std::istream& s) {
	readHeader(s);
	if (!checkHeader("SP"))
		return(false);

	unsigned int i;

	s.read((char*)&imgCount, sizeof(unsigned int));
	if (images != NULL)
		delete[] images;
	if (imgCount == 0)
		return(true);

	images = new Image[imgCount];
	for (i = 0; i < imgCount; i++)
		readImage(s, i);

	s.read((char*)&pal, sizeof(pal) * 256);

	return(true);
}

unsigned int RO::SPR::getImgCount() const {
	return(imgCount);
}

void RO::SPR::readImage(std::istream& s, unsigned int idx) {
	Image* img = &images[idx];
	
	unsigned char c;
	unsigned short length;
	
	char *dPtr;
	dPtr = (char*)&img->w;
	s.read(dPtr, 2);
	dPtr = (char*)&img->h;
	s.read(dPtr, 2);

	s.read((char*)&length, 2);
	if (s.eof()) {
		fprintf(stderr, "ERROR: Unexpected end of stream\n");
		return;
	}
	//printf("%u x %u (size: %u)\n", img->w, img->h, length);

	unsigned short pos;
	unsigned char* data = new unsigned char[length];
	s.read((char*)data, length);
	if (s.eof()) {
		fprintf(stderr, "ERROR: Unexpected end of stream\n");
		return;
	}
	std::stringstream out;
	
	for (pos = 0; pos < length; pos++) {
		c = data[pos];
		
		if (c == 0) {
			unsigned char len;
			len = data[pos+1];
			if (len != 0) {
				pos++;
				for (unsigned char j = 0; j < len; j++)
					out.write("\0", 1);
			}
		}
		else {
			out.write((char*)data + pos, 1);
		}
	}
	
	delete[] data;

	unsigned int size = out.tellp();
	img->data = new unsigned char[size];
	//printf(" bytes decoded: %u", size);
	//printf(" expected size: %u", img->w * img->h);
	//printf("\n");
	out.read((char*)img->data, size);
}

const RO::SPR::Image* RO::SPR::getFrame(const unsigned int& idx) const {
	if (idx >= imgCount)
		return(NULL);
		
	return(&images[idx]);
}

const RO::SPR::Pal* RO::SPR::getPal(const unsigned char& idx) const {
	return(&pal[idx]);
}

#pragma pack(push)
#pragma pack(1)
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
#pragma pack(pop)

bool RO::SPR::saveBMP(const unsigned int& idx, std::ostream& s) const {
	const Image* i = getFrame(idx);
	unsigned int k;
	int wscan = i->w;
	bmpHeader header;

	while (wscan % 4 != 0)
		wscan++;

	memset((char*)&header, 0, sizeof(bmpHeader));
	header.bmp.magic = 0x4D42; // 'BM'
	header.dib.size = 40;
	header.dib.planes = 1;
	header.dib.bpp = 8;
	header.dib.compression = 0;
	header.dib.vres = 1;
	header.dib.hres = 1;
	header.dib.colorcount = 256;
	header.dib.importantcolors = 0;
	header.dib.w = i->w;
	header.dib.h = i->h;
	header.dib.datasize = wscan * i->h;
	header.bmp.offset = sizeof(bmpHeader) + 256 * 4;
	header.bmp.size = header.bmp.offset + wscan * i->h;

	s.write((char*)&header, sizeof(bmpHeader));
	for (k = 0; k < 256; k++) {
		s.put(pal[k].b);
		s.put(pal[k].g);
		s.put(pal[k].r);
		if (k == 0) {
			s.put(0);
		}
		else
			s.put(pal[k].a);
	}

	char* dbuf = new char[wscan];
	memset(dbuf, 0, wscan);

	for (int ii = i->h - 1; ii >= 0; ii--) {
		memcpy(dbuf, &i->data[ii * i->w], i->w);
		s.write(dbuf, wscan);
	}

	delete[] dbuf;

	return(true);
}


bool RO::SPR::saveBMP(const unsigned int &idx, const std::string& fn) const {
	std::ofstream fp(fn.c_str(), std::ios_base::binary);
	bool ret = saveBMP(idx, fp);
	fp.close();
	return(ret);
}

