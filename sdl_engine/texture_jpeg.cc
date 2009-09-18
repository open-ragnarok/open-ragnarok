#include "stdafx.h"

#include "sdle/texture_jpeg.h"

#include <stdio.h>
#include "jpeglib.h"

namespace sdle {

Texture loadJPEGTexture(const char* fn) {
	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;

	memset(&cinfo, 0, sizeof(jpeg_decompress_struct));
	memset(&jerr, 0, sizeof(jpeg_error_mgr));

	Texture t;

	// Open file
    FILE * infile;
    if ((infile = fopen(fn, "rb")) == NULL) {
        fprintf(stderr, "can't open %s\n", fn);
        return(t);
    }
	// Start reading...
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	// Setup source
	jpeg_stdio_src(&cinfo, infile);

	// Get header
	jpeg_read_header(&cinfo, TRUE);

	// Sanity check
	int datasize = 3;
	bool grayscale = cinfo.jpeg_color_space == JCS_GRAYSCALE;
	bool rgb = cinfo.jpeg_color_space == JCS_RGB;
	bool ycbcr = cinfo.jpeg_color_space == JCS_YCbCr;
	if (!rgb && !grayscale && !ycbcr) {
		fprintf(stderr, "Unrecognized format!\n");
		jpeg_destroy_decompress(&cinfo);
		return(t);
	}
	if (grayscale) {
		datasize = 1;
	}

	// Start to work
	jpeg_start_decompress(&cinfo);

	// Create variables for reading and manipulating data
	Texture::Root* texp = new Texture::Root();
	unsigned char* line = new unsigned char[cinfo.output_width * datasize];
	unsigned char* data = new unsigned char[cinfo.output_width * cinfo.output_height * 4];
	unsigned char* dataptr = data;

	while (cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo, &line, 1);
		// Convert data
		switch (cinfo.jpeg_color_space) {
			case JCS_GRAYSCALE:
				{
					for (unsigned int i = 0; i < cinfo.output_width; i++) {
						dataptr[i * 4 + 0] = line[i];
						dataptr[i * 4 + 1] = line[i];
						dataptr[i * 4 + 2] = line[i];
						dataptr[i * 4 + 3] = 0;
					}
				}
				break;
			case JCS_RGB:
				{
					for (unsigned int i = 0; i < cinfo.output_width; i++) {
						dataptr[i * 4 + 0] = line[i * 3 + 0];
						dataptr[i * 4 + 1] = line[i * 3 + 1];
						dataptr[i * 4 + 2] = line[i * 3 + 2];
						dataptr[i * 4 + 3] = 0;
					}
				}
				break;
			case JCS_YCbCr:
				{
					float Y, Cb, Cr;
					int R, G, B;
					for (unsigned int i = 0; i < cinfo.output_width; i++) {
						Y = line[i * 3 + 0];
						Cb = line[i * 3 + 1];
						Cr = line[i * 3 + 2];

						/*
						R = (int)((298.082 * Y                + 408.583 * Cr ) / 256 - 222.921);
						G = (int)((298.082 * Y - 100.291 * Cb - 208.120 * Cr ) / 256 + 135.576);
						B = (int)((298.082 * Y + 516.412 * Cb                ) / 256 - 276.836);
						*/
						R = 1.164 * (Y - 16) + 1.596 * (Cr - 128);
						G = 1.164 * (Y - 16) - 0.813 * (Cr - 128) - 0.391 * (Cb - 128);
						B = 1.164 * (Y - 16) + 2.018 * (Cb - 128);

						if (R < 0) R = 0;
						if (R > 255) R = 255;
						if (G < 0) G = 0;
						if (G > 255) G = 255;
						if (B < 0) B = 0;
						if (B > 255) B = 255;

						dataptr[i * 4 + 0] = (unsigned char)R;
						dataptr[i * 4 + 1] = (unsigned char)G;
						dataptr[i * 4 + 2] = (unsigned char)B;
						dataptr[i * 4 + 3] = 0;
					}
				}
				break;
			default:
				// Shouldn't happen
				break;
		}
		// Increase pointer
		dataptr += cinfo.output_width * 4;
	}

	// Free used data
	texp->Create(data, cinfo.output_width, cinfo.output_height, 32);
	delete[] data;
	t = texp;

	// Finish loading texture
	jpeg_finish_decompress(&cinfo);

	// Destroys information
	jpeg_destroy_decompress(&cinfo);

	return(t);
}

}