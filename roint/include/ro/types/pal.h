/* $Id$ */
#ifndef __PAL_H
#define __PAL_H

#include "../ro_object.h"

namespace RO {
	/**
	 * Palette class.
	 * Contains an array of 256 palette entries.
	 * The first index is sometimes considered invisible.
	 *
	 * \ingroup ROInterface
	 */
	class ROINT_DLLAPI PAL : public Object {
	public:
#pragma pack(push,1)
		/**
		 * Palette entry.
		 * Represents a color with red, green and blue components.
		 */
		struct Pal {
			unsigned char r, g, b, reserved;
		};
#pragma pack(pop)

	protected:
		Pal m_pal[256];

	public:
		PAL();
		virtual ~PAL();

		virtual bool readStream(std::istream&);

		/** Returns the palette entry */
		const Pal* getPal(const unsigned char idx) const;
	};
}

#endif /* __PAL_H */

