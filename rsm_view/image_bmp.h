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

#ifndef __IMAGE_BMP_H
#define __IMAGE_BMP_H

#include "image.h"

#include <iostream>
#include <fstream>

/**
 * Reads a bitmap from a stream
 *
 * Note that this class is optimized for the bitmaps used in ragnarok. This is not an all-purpose bmp class (yet).
 */
class ImageBMP : public Image {
protected:
	ImageBMP(char*, int, int);
	
	bool ReadFromStream(std::istream& input, bool flipvertical = true);
	bool Read8BPP(std::istream& input, const bool& flipvertical);
	bool Read24BPP(std::istream& input, const bool& flipvertical);
	
public:

#pragma pack(push)
#pragma pack(1)
	typedef struct {
		unsigned char type[2];		/**< This specifies the type of file. This should be equal to BM */
		unsigned int file_size;	/**< This gives the bitmap size in bytes */
		unsigned short reserved_1;	/**< This is reserved and must be 0 */
		unsigned short reserved_2;	/**< This is also reserved and must also be 0 */
		unsigned int offset;		/**< This specifies the number of bytes from the starting of the bitmap to where the actual data part starts */
	} strFileHeader;

	typedef struct {
		unsigned int size;
		int width;						/* This specifies the width of bitmap in pixels */
		int height;						/* This specifies the height of bitmap in pixels */
		unsigned short color_planes;	/* This specifies the number of color planes */
		unsigned short bpp;				/* This specifies the bits per pixel i.e. the total number of colors in the bitmap */
		unsigned int compression;		/* This specifies the type of RLE compression in the bitmap. No compression means that the value will be 0. */
		unsigned long image_size;		/* This specifies the size of image in pixels i.e. width x height */
		int horizontal_resolution;		/* This specifies the horizontal resolution of image as pixels per meter */
		int vertical_resolution;		/* This specifies the vertical resolution of image as pixels per meter */
		unsigned int colors_used;		/* This specifies the colors used in the bitmap. For 4-bit image this value is 0 */
		unsigned int important_colors;	/* This specifies the minimum number of important colors in the bitmap */
	} strInfoHeader;
#pragma pack(pop)

	ImageBMP(const char* filename, bool flipverticale = true);
	ImageBMP(std::istream& input, bool flipvertical = true);
	ImageBMP(const unsigned char* data, const unsigned int& size, bool flipvertical = true);

	bool save(std::ostream&) const;
	
protected:
	unsigned long headerSize;		/* This specifies in bytes the size of the header. This must be equal to 40. */

	strFileHeader fileHeader;
	strInfoHeader infoHeader;
};


#endif /* __IMAGE_BMP_H */
