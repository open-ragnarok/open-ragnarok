/* $Id$ */
/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of The Open Ragnarok Project
    Copyright 2007 - 2009 The Open Ragnarok Team
    For the latest information visit http://www.open-ragnarok.org
    ------------------------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place - Suite 330, Boston, MA 02111-1307, USA, or go to
    http://www.gnu.org/copyleft/lesser.txt.
    ------------------------------------------------------------------------------------
*/
#ifndef __RO_TYPES_SPR_H
#define __RO_TYPES_SPR_H

#include "../ro_object.h"
#include "../ro_arr.h"
#include "pal.h"

#include <vector>

namespace RO {
	/**
	 * Sprite class.
	 * Contains palette images, RGBA images (v2.0+) and possibly a palette (v1.1+).
	 * Supported versions: v1.0 v1.1 v2.0 v2.1
	 * 
	 * \ingroup ROInterface
	 */
	class ROINT_DLLAPI SPR : public Object {
	public:
		/** Type of image */
		enum ImageType {
			PalType = 0,
			RgbaType = 1,
		};

#pragma pack(push,1)
		/** RGBA pixel. */
		struct Color {
			unsigned char a;
			unsigned char b;
			unsigned char g;
			unsigned char r;
			inline operator unsigned char* () { return(&a); }
			inline operator const unsigned char* () const { return(&a); }
		};

		/** Image data. */
		struct Image {
			ImageType type;
			unsigned short width;
			unsigned short height;
			union Data {
				unsigned char* pal; //< palette indexes; left to right, top to bottom ordering
				Color* rgba; //< color pixels; left to right, bottom to top ordering
			} data;
		};
#pragma pack(pop)

	protected:
		bool readImagePal(std::istream& s, unsigned int idx);
		bool readImageRgba(std::istream& s, unsigned int idx);
		void reset();

		Arr<Image> m_imagesPal;
		Arr<Image> m_imagesRgba;
		RO::PAL* m_pal;

	public:
		SPR();
		virtual ~SPR();

		virtual bool readStream(std::istream& s);

		/** Returns the number of images */
		unsigned int getImageCount(ImageType type) const;
		/** Returns the total number of images */
		unsigned int getImageCount() const;

		/** Returns the image data or NULL if not found */
		const Image* getImage(unsigned int idx, ImageType type) const;
		/** Returns the image data or NULL if not found */
		const Image* getImage(unsigned int idx) const;

		/** Returns the index of an image or -1 if not valid (pal images, then rgba images) */
		unsigned int getIndex(unsigned int idx, ImageType type) const;

		/** Returns the palette or NULL if not found */
		const RO::PAL* getPal() const;

		bool saveBMP(unsigned int idx, std::ostream& s, ImageType type=PalType, const RO::PAL* pal=NULL) const;
		bool saveBMP(unsigned int idx, const std::string& fn, ImageType type=PalType, const RO::PAL* pal=NULL) const;
		/** Saves all images of the same type in a single file */
		bool saveBMP(std::ostream& s, ImageType type=PalType, const RO::PAL* pal=NULL) const;
	};
}

#endif /* __RO_TYPES_SPR_H */

