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

#include "image_bmp.h"
#include <sstream>

#ifdef DEBUG
//#define BMP_DEBUG
#endif /* DEBUG */

ImageBMP::ImageBMP(const char* filename, bool flipvertical) : Image(NULL, 0, 0) {
	std::ifstream input;
	input.open(filename, std::ifstream::binary);
	assert(!input.fail() || !"Could not find file");
	ReadFromStream(input);
	input.close();
#ifdef BMP_DEBUG
	printf("[DEBUG] Loaded bitmap %s with dimensions %dx%d (%lu)\n", filename, width, height, dataSize);
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

	/*
	char buffer[2];
	input.read(buffer, 2);
	assert(buffer[0] == 'B' && buffer[1] == 'M' || !"Not a bitmap file");
	input.ignore(8);
	int dataOffset = readInt(input);
	*/
	
	//Read the header
	input.read((char*)&headerSize, sizeof(int));
	switch(headerSize) {
		case 40:
			infoHeader.size = headerSize;
			input.read((char*)&infoHeader.width, headerSize - sizeof(unsigned int));
			break;
		case 12:
			//OS/2 V1
			input.read((char*)&width, sizeof(int));
			input.read((char*)&height, sizeof(int));
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
			printf("invalid header size %d\n", headerSize);
			assert(!"Unknown bitmap format");
	}
	
	if (!infoHeader.width || !infoHeader.height) {
		return (false);
	}
	
	width = infoHeader.width;
	height = infoHeader.height;
	
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
	// RGBA colors[palsize]; // Round #3. Windows really should make my life easier...
	// printf("Palsize: %u\n", palsize);
	for (int i = 0; i < palsize; i++) {
		input.read((char*)&colors[i], 4);
	}
	
	int bpr = width;
	if (((width) % 4) > 0) {
		bpr += 4 - ((width) % 4);
		width -= 2;
	}
	int size = bpr * height; 
	input.seekg(fileHeader.offset, std::ios_base::beg);
	DynamicBlob pixels(size);
	input.read((char*)pixels.getBuffer(), size);

#if 1
	dataSize = width * height * 4;
	this->buffer = new char[dataSize];
	
	// printf("Reading BMP %d x %d\n", width, height);
	int idx, offset;

	unsigned char alpha;

/*
#ifdef _DEBUG
	{
		int x = 0;
		for (x = 0; x < 10; x++) {
			std::cout << "Color " << x << ": " << (int)colors[x].r << ", " << (int)colors[x].g << ", " << (int)colors[x].b << ", " << (int)colors[x].a << std::endl;
		}
	}
#endif
*/
	
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			//idx = pixels[(bpr << y) + x];
			idx = pixels[(bpr * y) + x];
#if 1
			// This hack is to simulate transparency based on Ragnarok textures using the #FF00FF color for transparent.
			if ((colors[idx].r == 255) && (colors[idx].g == 0) && (colors[idx].b == 255))
				alpha = 0;
			else
				alpha = 255;
#endif
			offset = 4 * (width * (flipvertical?(height-y-1):y) + x);
			this->buffer[offset  ] = colors[idx].r;
			this->buffer[offset+1] = colors[idx].g;
			this->buffer[offset+2] = colors[idx].b;
			this->buffer[offset+3] = alpha;
		}
	}
	bpp = 32;
#else
	dataSize = width * height * 3;
	//dataSize = bpr * height * 3;
	this->buffer = new char[dataSize];
	
	// printf("Reading BMP %d x %d\n", width, height);
	int idx, offset;
	
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			//idx = pixels[(bpr << y) + x];
			idx = pixels[(bpr * y) + x];
			#if 0
			printf("%d x %d (%d) = ", x, y, (bpr * y + x));
			printf("%d", idx);
			printf(" (%d, %d, %d)\n", colors[pixels[idx]].b, colors[pixels[idx]].g, colors[pixels[idx]].r);
			#endif
			offset = 3 * (width * (flipvertical?(height-y-1):y) + x);
			this->buffer[offset  ] = colors[idx].r;
			this->buffer[offset+1] = colors[idx].g;
			this->buffer[offset+2] = colors[idx].b;
		}
	}
	bpp = 24;
#endif
	delete[] colors;
	return(true);
}

bool ImageBMP::Read24BPP(std::istream& input, const bool& flipvertical) {
	int bytesPerRow = ((width * 3 + 3) / 4) * 4 - (width * 3 % 4);
	int size = bytesPerRow * height;
	DynamicBlob pixels(size);
	
	//Read the data
	input.seekg(fileHeader.offset, std::ios_base::beg);
	input.read(pixels, size);
	
	//Get the data into the right format
	dataSize = width * height * 3;
	this->buffer = new char[dataSize];
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			for(int c = 0; c < 3; c++) {
				this->buffer[3 * (width * y + x) + c] =
					pixels[bytesPerRow * (flipvertical?(height-y-1):y) + 3 * x + (2 - c)];
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

