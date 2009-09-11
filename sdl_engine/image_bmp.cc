/* $Id$ */
#include "stdafx.h"
#include "sdle/image_bmp.h"
#include <sstream>

#ifdef DEBUG
//#define BMP_DEBUG
#endif /* DEBUG */

namespace sdle {
ImageBMP::ImageBMP(const char* filename, bool flipvertical) : Image(NULL, 0, 0) {
	std::ifstream input;
	input.open(filename, std::ifstream::binary);
	assert(!input.fail() || !"Could not find file");
	ReadFromStream(input);
	input.close();
#ifdef BMP_DEBUG
	printf("[DEBUG] Loaded bitmap %s with dimensions %dx%d (%lu)\n", filename, m_width, m_height, dataSize);
#endif
}

ImageBMP::ImageBMP(std::istream& input, bool flipvertical) : Image(NULL, 0, 0) {
	ReadFromStream(input);
}

ImageBMP::ImageBMP(const unsigned char* data, const unsigned int& size, bool flipvertical) : Image(NULL, 0, 0) {
	std::stringstream s;
	s.write((const char*)data, size);
	ReadFromStream(s, flipvertical);
}

bool ImageBMP::ReadFromStream(std::istream& input, bool flipvertical) {
	using namespace std;
	
	input.read((char*)&fileHeader, sizeof(strFileHeader));
	if ((fileHeader.type[0] != 'B') || (fileHeader.type[1] != 'M')) {
#ifdef _DEBUG
		fprintf(stderr, "Invalid BMP header: %c%c\n", fileHeader.type[0], fileHeader.type[1]);
		if (input.eof()) {
			fprintf(stderr, "\tEOF()\n");
		}
#endif
		return(false);
	}

	//Read the header
	input.read((char*)&headerSize, sizeof(int));
	switch(headerSize) {
		case 40:
			infoHeader.size = headerSize;
			input.read((char*)&infoHeader.width, headerSize - sizeof(unsigned int));
			break;
		case 12:
			//OS/2 V1
			input.read((char*)&m_width, sizeof(int));
			input.read((char*)&m_height, sizeof(int));
			input.ignore(2);
			short bpp;
			input.read((char*)&bpp, sizeof(short));
			assert(bpp == 24 || !"Image is not 24 bits per pixel");
			break;
		case 64:
			//OS/2 V2
			assert(!"Can't load OS/2 V2 bitmaps");
			break;
		case 108:
			//Windows V4
			assert(!"Can't load Windows V4 bitmaps");
			break;
		case 124:
			//Windows V5
			assert(!"Can't load Windows V5 bitmaps");
			break;
		default:
			printf("invalid header size %ld\n", headerSize);
			assert(!"Unknown bitmap format");
	}
	
	if (!infoHeader.width || !infoHeader.height) {
		return (false);
	}
	
	m_width = infoHeader.width;
	m_height = infoHeader.height;
	
	switch (infoHeader.bpp) {
		case 8:
			return Read8BPP(input, flipvertical);
			break;
		case 24:
			return Read24BPP(input, flipvertical);
			break;
		default:
			printf("[BITMAP ERROR] Unsupported bpp: %d\n", infoHeader.bpp);
	}
	
	return(false);

}

bool ImageBMP::Read8BPP(std::istream& input, const bool& flipvertical) {
	int palsize = 256;
	if (infoHeader.colors_used > 0)
		palsize = infoHeader.colors_used;
	
	typedef struct {
		unsigned char b, g, r, a;
	} RGBA;
	RGBA *colors = new RGBA[palsize];

	for (int i = 0; i < palsize; i++) {
		input.read((char*)&colors[i], 4);
	}
	
	int bpr = m_width;
	if (((m_width) % 4) > 0) {
		bpr += 4 - ((m_width) % 4);
		m_width -= 2;
	}
	int size = bpr * m_height; 
	input.seekg(fileHeader.offset, std::ios_base::beg);
	DynamicBlob pixels(size);
	input.read(pixels.getBuffer(), size);

#if 1
	dataSize = m_width * m_height * 4;
	this->buffer = new char[dataSize];
	
	// printf("Reading BMP %d x %d\n", width, height);
	unsigned int idx;
	int offset;
	unsigned char alpha;

	unsigned char* px = (unsigned char*)pixels.getBuffer();

	for(int y = 0; y < m_height; y++) {
		for(int x = 0; x < m_width; x++) {
			//idx = pixels[(bpr << y) + x];
			idx = px[(bpr * y) + x];
#if 1
			// This hack is to simulate transparency based on Ragnarok textures using the #FF00FF color for transparent.
			if ((colors[idx].r == 255) && (colors[idx].g == 0) && (colors[idx].b == 255))
				alpha = 0;
			else
				alpha = 255;
#endif
			offset = 4 * (m_width * (flipvertical?(m_height-y-1):y) + x);
			this->buffer[offset  ] = colors[idx].r;
			this->buffer[offset+1] = colors[idx].g;
			this->buffer[offset+2] = colors[idx].b;
			this->buffer[offset+3] = alpha;
		}
	}
	m_bpp = 32;
#else
	dataSize = m_width * m_height * 3;
	this->buffer = new char[dataSize];
	
	// printf("Reading BMP %d x %d\n", width, height);
	int idx, offset;
	
	for(int y = 0; y < m_height; y++) {
		for(int x = 0; x < m_width; x++) {
			//idx = pixels[(bpr << y) + x];
			idx = pixels[(bpr * y) + x];
			#if 0
			printf("%d x %d (%d) = ", x, y, (bpr * y + x));
			printf("%d", idx);
			printf(" (%d, %d, %d)\n", colors[pixels[idx]].b, colors[pixels[idx]].g, colors[pixels[idx]].r);
			#endif
			offset = 3 * (m_width * (flipvertical?(m_height-y-1):y) + x);
			this->buffer[offset  ] = colors[idx].r;
			this->buffer[offset+1] = colors[idx].g;
			this->buffer[offset+2] = colors[idx].b;
		}
	}
	m_bpp = 24;
#endif
	delete[] colors;
	return(true);
}

bool ImageBMP::Read24BPP(std::istream& input, const bool& flipvertical) {
	int bytesPerRow = ((m_width * 3 + 3) / 4) * 4 - (m_width * 3 % 4);
	int size = bytesPerRow * m_height;
	DynamicBlob pixels(size);
	
	//Read the data
	input.seekg(fileHeader.offset, std::ios_base::beg);
	input.read(pixels, size);
	
	//Get the data into the right format
	dataSize = m_width * m_height * 3;
	this->buffer = new char[dataSize];
	for(int y = 0; y < m_height; y++) {
		for(int x = 0; x < m_width; x++) {
			for(int c = 0; c < 3; c++) {
				this->buffer[3 * (m_width * y + x) + c] =
					pixels[bytesPerRow * (flipvertical?(m_height-y-1):y) + 3 * x + (2 - c)];
			}
		}
	}

	return(true);
}

bool ImageBMP::save(std::ostream& out) const {
	std::cerr << "ImageBMP::save() not yet implemented" << std::endl;
	return(false);
}
	
ImageBMP::ImageBMP(char*p, int w, int h) : Image(p, w, h) {
}
}
