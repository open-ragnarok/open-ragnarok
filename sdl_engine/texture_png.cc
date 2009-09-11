/* $Id$
 * texture_png.cc
 *
 *  Created on: 2009-09-09
 *      Author: Sergio Moura
 *    Revision: $Rev$
 */
#include "stdafx.h"
#include "sdle/texture_png.h"

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
