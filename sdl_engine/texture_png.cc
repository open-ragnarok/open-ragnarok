/* $Id$
 * texture_png.cc
 *
 *  Created on: 2009-09-09
 *      Author: Sergio Moura
 *    Revision: $Rev$
 */
#include "stdafx.h"
#include "sdle/texture_png.h"

#include <sstream>
#include <iostream>

namespace sdle {
Texture loadPngTexture(const char* filename) {
	PngLoader png(filename);
	if (!png.Valid()) {
		return(Texture());
	}

	Texture::Root* texp = new Texture::Root();

	unsigned char* data = new unsigned char[png.width * png.height * 4];

	unsigned char* dataptr = data;
	for (unsigned int i = 0; i < png.height; i++) {
		png_read_row(png.png_ptr, (png_bytep)dataptr, png_bytep_NULL);
		dataptr += png.width * 4;
	}

	texp->Create(data, png.width, png.height, 32);

	delete[] data;

	Texture ret(texp);
	return(ret);
}

#define PNGSIGSIZE 8

void userReadData(png_structp pngPtr, png_bytep data, png_size_t length) {
    //Here we get our IO pointer back from the read struct.
    //This is the parameter we passed to the png_set_read_fn() function.
    //Our std::istream pointer.
    png_voidp a = png_get_io_ptr(pngPtr);
    //Cast the pointer to std::istream* and read 'length' bytes into 'data'
    ((std::istream*)a)->read((char*)data, length);
}

Texture loadPngTexture(const char* buf, unsigned int buflen) {
	// Thanks to: http://www.piko3d.com/?page_id=68
	std::stringstream source;
	source.write(buf, buflen);

    //Allocate a buffer of 8 bytes, where we can put the file signature.
    png_byte pngsig[PNGSIGSIZE];
    int is_png = 0;

    //Read the 8 bytes from the stream into the sig buffer.
    source.read((char*)pngsig, PNGSIGSIZE);

    //Check if the read worked...
    if (!source.good())
		return Texture();

    //Let LibPNG check the sig. If this function returns 0, everything is OK.
    is_png = png_sig_cmp(pngsig, 0, PNGSIGSIZE);

	if (is_png != 0) {
		return(Texture());
	}

	//Here we create the png read struct. The 3 NULL's at the end can be used
	//for your own custom error handling functions, but we'll just use the default.
	//if the function fails, NULL is returned. Always check the return values!
	png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!pngPtr) {
		std::cerr << "ERROR: Couldn't initialize png read struct" << std::endl;
		return(Texture()); //Do your own error recovery/handling here
	}

	//Here we create the png info struct.
	//Note that this time, if this function fails, we have to clean up the read struct!
	png_infop infoPtr = png_create_info_struct(pngPtr);
	if (!infoPtr) {
		std::cerr << "ERROR: Couldn't initialize png info struct" << std::endl;
		png_destroy_read_struct(&pngPtr, (png_infopp)0, (png_infopp)0);
		return(Texture()); //Do your own error recovery/handling here
	}

	png_set_read_fn(pngPtr,(voidp)&source, userReadData);

    //Set the amount signature bytes we've already read:
    //We've defined PNGSIGSIZE as 8;
    png_set_sig_bytes(pngPtr, PNGSIGSIZE);

    //Now call png_read_info with our pngPtr as image handle, and infoPtr to receive the file info.
    png_read_info(pngPtr, infoPtr);

    png_uint_32 imgWidth =  png_get_image_width(pngPtr, infoPtr);
    png_uint_32 imgHeight = png_get_image_height(pngPtr, infoPtr);

    //bits per channel
    png_uint_32 bitdepth   = png_get_bit_depth(pngPtr, infoPtr);
    //Number of channels
    png_uint_32 channels   = png_get_channels(pngPtr, infoPtr);
    //Color type. (RGB, RGBA, Luminance, luminance alpha... palette... etc)
    png_uint_32 color_type = png_get_color_type(pngPtr, infoPtr);

   switch (color_type) {
        case PNG_COLOR_TYPE_PALETTE:
            png_set_palette_to_rgb(pngPtr);
            break;
        case PNG_COLOR_TYPE_GRAY:
            if (bitdepth < 8)
            png_set_gray_1_2_4_to_8(pngPtr);
            break;
    }

    /*if the image has a transperancy set.. convert it to a full Alpha channel..*/
    if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(pngPtr);

    //We don't support 16 bit precision.. so if the image Has 16 bits per channel
        //precision... round it down to 8.
    if (bitdepth == 16)
        png_set_strip_16(pngPtr);

	Texture::Root* texp = new Texture::Root();

	unsigned char* data = new unsigned char[imgWidth * imgHeight * 4];

	unsigned char* dataptr = data;
	for (unsigned int i = 0; i < imgHeight; i++) {
		png_read_row(pngPtr, (png_bytep)dataptr, png_bytep_NULL);
		dataptr += imgWidth * 4;
	}

	texp->Create(data, imgWidth, imgHeight, 32);

	delete[] data;

	Texture ret(texp);


/*
    //Array of row pointers. One for every row.
    png_bytep* rowPtrs = new png_bytep[imgHeight];

    //Alocate a buffer with enough space.
    //(Don't use the stack, these blocks get big easilly)
    const char* data = new char[imgWidth * imgHeight * bitdepth * channels / 8];
    //This is the length in bytes, of one row.
    const unsigned int stride = imgWidth * bitdepth * channels / 8;

    //A little for-loop here to set all the row pointers to the starting
    //Adresses for every row in the buffer
    for (size_t i =0; i < bufferHeight; i++) {
        //Set the pointer to the data pointer + i times the row stride.
        rowPtrs[i] = (png_bytep)data + (i * stride);
    }

    //And here it is! The actuall reading of the image!
    //Read the imagedata and write it to the adresses pointed to
    //by rowptrs (in other words: our image databuffer)
    png_read_image(pngPtr, rowPtrs);

    //Delete the row pointers array....

    delete[] (png_bytep)rowPtrs;
*/

    //And don't forget to clean up the read and info structs !

    png_destroy_read_struct(&pngPtr, &infoPtr,(png_infopp)0);
	return(ret);
}

void user_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
	printf("PNG Warning: %s\n", warning_msg);
}

PngLoader::PngLoader(const char* filename) {
	m_valid = false;

	unsigned int sig_read = 0;
	int bit_depth, color_type, interlace_type;

	if ((fp = fopen(filename, "rb")) == NULL) {
		return;
	}

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (png_ptr == NULL) {
		fclose(fp);
		return;
	}
	png_set_error_fn(png_ptr, (png_voidp) NULL, (png_error_ptr) NULL, user_warning_fn);
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		return;
	}
	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, sig_read);
	png_read_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, int_p_NULL, int_p_NULL);

	//ret->widthpot = getNextPower2(ret->width);
	//ret->heightpot = getNextPower2(ret->height);

	png_set_strip_16(png_ptr);
	png_set_packing(png_ptr);
	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png_ptr);
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_gray_1_2_4_to_8(png_ptr);
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png_ptr);
	png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);

	m_valid = true;
}

PngLoader::~PngLoader() {
	if (m_valid) {
		png_read_end(png_ptr, info_ptr);
		png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
		fclose(fp);
	}
}

bool PngLoader::Valid() const {
	return(m_valid);
}
}
