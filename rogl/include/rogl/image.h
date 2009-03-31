/* $Id$ */
#ifndef __ROGL_IMAGE_H
#define __ROGL_IMAGE_H

#include "rogl_import.h"
#include "blob.h"

namespace rogl {
	typedef unsigned char img_t;

	/**
	 * This class is an class that holds the generic info for a image.
	 * Image loads should be implemented in classes derived from this one.
	 */
	class ROGL_DLLAPI Image : public DynamicBlobUChar {
	protected:
		Image(const img_t*, int w, int h, int bpp = 24);
		
		int width, height, bpp;
	public:
		Image(const Image&);
		virtual ~Image();
		
		int glDrawType;
		
		int getWidth() const;
		int getHeight() const;
		int getBpp() const;

		bool resizeW(const int&);
		bool resizeH(const int&);
	};
}

#endif /* __ROGL_IMAGE_H */
