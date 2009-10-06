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
#ifndef __SPR_H
#define __SPR_H

#include "../ro_object.h"
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
		typedef enum {
			IT_PAL = 0,
			IT_RGBA = 1,
		} ImageType;
#pragma pack(push,1)
		/**
		 * Image data.
		 * For palette images, data is an array of palette indexes (left to right, top to bottom ordering).
		 * For RGBA images, data is an array of raw colors {uchar a,b,g,r} (left to right, bottom to top ordering).
		 */
		typedef struct Image {
			unsigned short w, h;
			unsigned char* data;
		};
#pragma pack(pop)

	protected:
		void readImagePal(std::istream& s, const unsigned short idx);
		void readImageRgba(std::istream& s, const unsigned short idx);
		void reset();

		unsigned short m_imgCountPal;
		unsigned short m_imgCountRgba;
		Image* m_imagesPal;
		Image* m_imagesRgba;
		RO::PAL* m_pal;

	public:
		SPR();
		virtual ~SPR();

		virtual bool readStream(std::istream& s);

		/** Returns the number of images */
		unsigned short getImgCount(const ImageType type) const;

		/** Returns the image data or NULL if not found */
		const Image* getImage(const unsigned short idx, const ImageType type) const;

		/** Returns the palette or NULL if not found */
		const RO::PAL* getPal() const;

		bool saveBMP(const unsigned short idx, const ImageType type, std::ostream& s, const RO::PAL* pal=NULL) const;
		bool saveBMP(const unsigned short idx, const ImageType type, const std::string& fn, const RO::PAL* pal=NULL) const;
		/** Saves all BMPs in a single file */
		bool saveBMP(std::ostream& s, ImageType type = IT_PAL, const RO::PAL* pal=NULL) const;

		// deprecated, backward compatible functions:
		unsigned int getImgCount() const {
			return getImgCount(IT_PAL);
		}
		bool saveBMP(const unsigned int& idx, std::ostream& s) const {
			return saveBMP(idx, IT_PAL, s);
		}
		bool saveBMP(const unsigned int& idx, const std::string& fn) const {
			return saveBMP(idx, IT_PAL, fn);
		}
		const Image* getFrame(const unsigned int& idx) const {
			return getImage(idx, IT_PAL);
		}
		typedef RO::PAL::Pal Pal;
		const Pal* getPal(const unsigned char& idx) const {
			if (m_pal != NULL)
				return(m_pal->getPal(idx));
			return(NULL);
		}
	};
}

#endif /* __SPR_H */

