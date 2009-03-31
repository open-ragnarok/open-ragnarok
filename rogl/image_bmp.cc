/* $Id: image_bmp.cc 53 2009-02-26 21:59:31Z sergio $ */

#include "stdafx.h"
#include "rogl/image_bmp.h"

#include <iostream>
#include <sstream>

rogl::ImageBMP::ImageBMP(const std::string& filename, bool flipvertical) : Image(NULL, 0, 0) {
	std::ifstream input;
	input.open(filename.c_str(), std::ios_base::binary);
	assert(!input.fail() || !"Could not find file");
	ReadFromStream(input);
	input.close();
}

rogl::ImageBMP::ImageBMP(std::istream& input, bool flipvertical) : Image(NULL, 0, 0) {
	ReadFromStream(input);
}

rogl::ImageBMP::ImageBMP(const img_t* data, const unsigned int& size, bool flipvertical) : Image(NULL, 0, 0) {
	std::stringstream s;
	s.write((const char*)data, size);
	ReadFromStream(s, flipvertical);
}

bool rogl::ImageBMP::ReadHeader(std::istream& input) {
	unsigned int hsize;
	input.read((char*)&hsize, sizeof(unsigned int));

	switch(hsize) {
		case 40:
			// Windows V3
			_htype = HEAD_WINV3;
			_head_winv3.headerSize = hsize;
			input.read((char*)&_head_winv3.width, hsize - sizeof(unsigned int));

			width = _head_winv3.width;
			height = _head_winv3.height;
			bpp = _head_winv3.bpp;
			break;
		case 12:
			// OS/2 V1
			_htype = HEAD_OS2V1;
			_head_os2v1.headerSize = hsize;
			input.read((char*)&_head_os2v1.width, hsize - sizeof(unsigned int));

			width = _head_os2v1.width;
			height = _head_os2v1.height;
			bpp = _head_os2v1.bpp;
			break;
		default:
			_htype = HEAD_UNKNOWN;
			fprintf(stderr, "Inrecognized BMP header size: %d\n", hsize);
			width = 0;
			height = 0;
			bpp = 0;
			return(false);
	}
	return(true);
}


void rogl::ImageBMP::ReadFromStream(std::istream& input, bool flipvertical) {
	using namespace std; 

	char buffer[2];
	input.read(buffer, 2);
	// printf("BMP: %c%c\n", buffer[0], buffer[1]);
	if (!(buffer[0] == 'B' || buffer[1] == 'M')) {
		fprintf(stderr, "Not a bitmap file");
		return;
	}
	input.ignore(8);
	input.read((char*)&dataOffset, sizeof(int));
	
	//Read the header
	if (!ReadHeader(input)) {
		return;
	}

	if (bpp == 8) {
		Read8bpp(input);
	}
	else if (bpp == 24) {
		flipvertical = false;
		int bytesPerRow = ((width * 3 + 3) / 4) * 4 - (width * 3 % 4);
		int size = bytesPerRow * height;
		DynamicBlob pixels(size);
		
		//Read the data
		input.seekg(dataOffset, ios_base::beg);
		input.read((char*)*pixels, size);
		
		//Get the data into the right format
		dataSize = width * height * 3;
		this->buffer = new img_t[dataSize];
		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
				for(int c = 0; c < 3; c++) {
					this->buffer[3 * (width * y + x) + c] =
						pixels[bytesPerRow * (flipvertical?(height-y-1):y) + 3 * x + (2 - c)];
				}
			}
		}
	}
}

int rogl::ImageBMP::getColors() const {
	if (_htype == HEAD_WINV3)
		return(_head_winv3.colors);
	
	return(256);
}

bool rogl::ImageBMP::Read8bpp(std::istream& input) {
		// Read 8bpp image
		//glDrawType = GL_BGRA;
		typedef struct {
			unsigned char b, g, r, a;
		} RGBA;

		int palsize = getColors();
		if (palsize == 0)
			palsize = 256;

		RGBA *colors = new RGBA[palsize];
		// RGBA colors[palsize]; // Round #3. Windows really should make my life easier...
		// printf("Palsize: %u\n", palsize);
		for (int i = 0; i < palsize; i++)
			input.read((char*)&colors[i], 4);
		
		int bpr = width;
		if (((width) % 4) > 0) {
			bpr += 4 - ((width) % 4);
			//width -= 2;
		}
		int size = bpr * height; 
		input.seekg(dataOffset, std::ios_base::beg);
		DynamicBlob pixels(size);
		input.read((char*)*pixels, size);
		
		int dataSize = width * height * 4;
		//this->buffer = new img_t[dataSize];
		setSize(dataSize);
		
		int idx, offset;
		unsigned char alpha;
		
		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
				//idx = pixels[(bpr << y) + x];
				idx = pixels[(bpr * y) + x];
				#if 0
				printf("%d x %d (%d) = ", x, y, (bpr * y + x));
				printf("%d", idx);
				printf(" (%d, %d, %d)\n", colors[pixels[idx]].b, colors[pixels[idx]].g, colors[pixels[idx]].r);
				#endif
				//offset = 3 * (width * (flipvertical?(height-y-1):y) + x);
				offset = 4 * (width * y + x);
				if (colors[idx].r == 255 && colors[idx].b == 255 && colors[idx].g == 0) {
					alpha = 0;
				}
				else {
					alpha = 255;
				}
				this->buffer[offset  ] = colors[idx].r;
				this->buffer[offset+1] = colors[idx].g;
				this->buffer[offset+2] = colors[idx].b;
				this->buffer[offset+3] = alpha;
			}
		}
		delete[] colors;
		bpp = 32;

		return(true);
}

	
rogl::ImageBMP::ImageBMP(const img_t *p, int w, int h) : Image(p, w, h) {
}
