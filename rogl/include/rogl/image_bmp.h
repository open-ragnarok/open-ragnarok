/* $Id: image_bmp.h 39 2009-02-06 00:52:25Z sergio $ */
#ifndef __ROGL_IMAGE_BMP_H
#define __ROGL_IMAGE_BMP_H

#include "image.h"

#include <iostream>
#include <fstream>

namespace rogl {
	class ROGL_DLLAPI ImageBMP : public Image {
	protected:
		ImageBMP(const img_t*, int, int);
		
		void ReadFromStream(std::istream& input, bool flipvertical = true);
		bool ReadHeader(std::istream& input);
		bool Read8bpp(std::istream& input);
		int getColors() const;

		enum header_type {
			HEAD_UNKNOWN,
			HEAD_OS2V1,
			HEAD_WINV3
		};

		header_type _htype;

#ifdef _MSC_VER
	#pragma pack(push)
	#pragma pack(1)
#endif
		struct HeaderV3 {
			unsigned int headerSize;
			int width;
			int height;
			short planes; // Always 1
			short bpp;
			int compression;
			int img_size;
			int hres;
			int vres;
			int colors;
			int important_colors;
		};

		struct HeaderOS2V1 {
			unsigned int headerSize;
			short width;
			short height;
			short planes; // Always 1
			short bpp;
		};
#ifdef _MSC_VER
	#pragma pack(pop)
#endif

		int dataOffset;

		HeaderV3 _head_winv3;
		HeaderOS2V1 _head_os2v1;
	public:
		ImageBMP(const std::string& filename, bool flipverticale = true);
		ImageBMP(std::istream& input, bool flipvertical = true);
		ImageBMP(const img_t* data, const unsigned int& size, bool flipvertical = true);
	};
}

#endif /* __IMAGE_BMP_H */
