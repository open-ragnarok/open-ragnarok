/* $Id$
 * font.h
 *
 *  Created on: 2009-09-09
 *      Author: Sergio Moura
 *    Revision: $Rev$
 */

#ifndef __SDLENGINE_FONT_H
#define __SDLENGINE_FONT_H

#include "sdle_import.h"
#include "texture.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <vector>
#include <cstdarg>

namespace sdle {

class SDLENGINE_DLLAPI Font {
protected:
	static FT_Library library;
	static void initLibrary();
	static void killLibrary();
	static unsigned int m_count;

	bool m_valid;

	Texture m_texture;

	/** Character that the reading started from */
	unsigned int m_startchar;

	/** Number of characters stored in our texture */
	unsigned int m_charcount;

	/** The maximum text height */
	unsigned int m_height;

	/** The width for each character */
	unsigned int *m_widths;

	/** The GL list of each character loaded */
	unsigned int m_gllists;
public:
	Font();
	~Font();

	/**
	 * Opens a given filename and read 'charcount' glyphs starting inposition 'startchar'
	 * @param fn the file to load from
	 * @param size the font size
	 * @param startchar the character to start loading from (default: space ' ')
	 * @param charcount number of chars to read from the font file
	 */
	bool open(const char* fn, unsigned int size, unsigned int startchar = 32, unsigned int charcount = 96);
	bool valid() const;

	/** Erases the font */
	void release();

	/** Draws the text in the given xy position */
	void drawText(float x, float y, const char *str, ...) const;
};

}

#endif /* __SDLENGINE_FONT_H */
