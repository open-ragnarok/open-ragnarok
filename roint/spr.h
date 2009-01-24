/* $Id$ */
#ifndef __SPR_H
#define __SPR_H

#include "object.h"

#include <vector>

namespace RO {
	/**
	 * Sprite libraries
	 * Each file contain one or more sprite (bitmaps) for numerous goals.
	 *
	 * \ingroup ROInterface
	 */
	class ROINT_DLLAPI SPR : public Object {
	public:
#pragma pack(push)
#pragma pack(1)
		// Structures
		typedef struct Image {
			unsigned short w, h;
			unsigned char* data;
		};

		typedef struct Pal {
			unsigned char r, g, b, a;
		};
#pragma pack(pop)

	protected:
		unsigned int imgCount;
		void readImage(std::istream&, unsigned int idx);

		Image* images;
		Pal pal[256];

	public:
		SPR();
		virtual ~SPR();
		
		virtual bool readStream(std::istream&);
		unsigned int getImgCount() const;
		bool saveBMP(const unsigned int& idx, std::ostream&) const;
		bool saveBMP(const unsigned int& idx, const std::string&) const;

		const Image* getFrame(const unsigned int& idx) const;
		const Pal* getPal(const unsigned char& idx) const;
	};
}

#endif /* __SPR_H */

