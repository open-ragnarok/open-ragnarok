#include "stdafx.h"

#include "sdle/texture_jpeg.h"

#include <stdio.h>
#include "jpeglib.h"

namespace sdle {

typedef struct {
  struct jpeg_source_mgr pub;	/* public fields */
  const unsigned char *fulldata;
  unsigned int datasize;
  unsigned int loaded; // Amount of data loaded

  FILE * infile;		/* source stream */
  JOCTET * buffer;		/* start of buffer */
} jpeg_memory_source_mgr;

typedef jpeg_memory_source_mgr * jpeg_memory_source_ptr;

#define INPUT_BUF_SIZE  4096	/* choose an efficiently fread'able size */

void JPM_init_source (j_decompress_ptr cinfo) {
  jpeg_memory_source_ptr src = (jpeg_memory_source_ptr) cinfo->src;

  /* We reset the empty-input-file flag for each image,
   * but we don't clear the input buffer.
   * This is correct behavior for reading a series of images from one source.
   */
  src->loaded = 0;
}

#ifndef _MSC_VER
boolean JPM_fill_input_buffer (j_decompress_ptr cinfo) {
#else
unsigned char JPM_fill_input_buffer (j_decompress_ptr cinfo) {
#endif
	jpeg_memory_source_ptr src = (jpeg_memory_source_ptr) cinfo->src;

	int btr = INPUT_BUF_SIZE;
	if (btr > (src->datasize - src->loaded)) {
		btr = src->datasize - src->loaded;
	}

	if (btr <= 0) {
		//WARNMS(cinfo, JWRN_JPEG_EOF);
		/* Insert a fake EOI marker */
		src->buffer[0] = (JOCTET) 0xFF;
		src->buffer[1] = (JOCTET) JPEG_EOI;
		btr = 2;
	}
	else {
		memcpy(src->buffer, src->fulldata + src->loaded, btr);
		src->loaded += btr;
	}

	src->pub.next_input_byte = src->buffer;
	src->pub.bytes_in_buffer = btr;

	return TRUE;
}


void JPM_skip_input_data (j_decompress_ptr cinfo, long num_bytes) {
  jpeg_memory_source_ptr src = (jpeg_memory_source_ptr) cinfo->src;

  /* Just a dumb implementation for now.  Could use fseek() except
   * it doesn't work on pipes.  Not clear that being smart is worth
   * any trouble anyway --- large skips are infrequent.
   */
  if (num_bytes > 0) {
    while (num_bytes > (long) src->pub.bytes_in_buffer) {
      num_bytes -= (long) src->pub.bytes_in_buffer;
      JPM_fill_input_buffer(cinfo);
      /* note we assume that fill_input_buffer will never return FALSE,
       * so suspension need not be handled.
       */
    }
    src->pub.next_input_byte += (size_t) num_bytes;
    src->pub.bytes_in_buffer -= (size_t) num_bytes;
  }
}


void JPM_term_source (j_decompress_ptr cinfo) {
  /* no work necessary here */
}

void jpeg_memory_loader (j_decompress_ptr cinfo, const unsigned char* data, unsigned int datasize) {
	jpeg_memory_source_ptr src;

	if (cinfo->src == NULL) {	/* first time for this JPEG object? */
		cinfo->src = (struct jpeg_source_mgr *)
			(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(jpeg_memory_source_mgr));

		src = (jpeg_memory_source_ptr) cinfo->src;

		src->buffer = (JOCTET *)
			(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT, INPUT_BUF_SIZE * sizeof(JOCTET));
	}

	src = (jpeg_memory_source_ptr) cinfo->src;
	src->fulldata = data;
	src->datasize = datasize;
	src->pub.init_source = JPM_init_source;
	src->pub.fill_input_buffer = JPM_fill_input_buffer;
	src->pub.skip_input_data = JPM_skip_input_data;
	src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
	src->pub.term_source = JPM_term_source;
	src->pub.bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
	src->pub.next_input_byte = NULL; /* until buffer loaded */
}


Texture loadJPEGTexture(const unsigned char* buf, unsigned long buflen) {
	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;

	memset(&cinfo, 0, sizeof(jpeg_decompress_struct));
	memset(&jerr, 0, sizeof(jpeg_error_mgr));

	Texture t;


	// Start reading...
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	// Setup source
	jpeg_memory_loader(&cinfo, buf, buflen);
	//jpeg_stdio_src(&cinfo, infile);

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
						/*
						R = 1.164 * (Y - 16) + 1.596 * (Cr - 128);
						G = 1.164 * (Y - 16) - 0.813 * (Cr - 128) - 0.391 * (Cb - 128);
						B = 1.164 * (Y - 16) + 2.018 * (Cb - 128);
						*/
						R = (int)Y;
						G = (int)Cb;
						B = (int)Cr;

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
		fclose(infile);
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

						R = (int)Y;
						G = (int)Cb;
						B = (int)Cr;

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

	fclose(infile);

	return(t);
}

}
