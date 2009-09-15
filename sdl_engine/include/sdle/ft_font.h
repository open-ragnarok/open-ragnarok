/* $Id$
 * font.h
 *
 *  Created on: 2009-09-09
 *      Author: Sergio Moura
 *    Revision: $Rev$
 */

#ifndef __SDLENGINE_FT_FONT_H
#define __SDLENGINE_FT_FONT_H

#include "sdle_import.h"
#include "texture.h"
#include "font.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <vector>
#include <cstdarg>

namespace sdle {

/** Freetype font implementation */
class SDLENGINE_DLLAPI FTFont : public Font {
private:
	/** The freetype library */
	static FT_Library library;
	
	/** Initializes the freetype library */
	static void initLibrary();
	
	/** Terminates the freetype library */
	static void killLibrary();

	/**
	 * How many instances of FTFont are in use
	 * When no more instances of FTFont are present, the freetype library is destroyed.
	 */
	static unsigned int m_count;

protected:
	/** Texture used by this font */
	Texture m_texture;

	/** Character that the reading started from */
	unsigned short m_startchar;

	/** Number of characters stored in our texture */
	unsigned short m_charcount;

	/** The maximum text height */
	unsigned int m_height;

	/** The width for each character */
	unsigned int *m_widths;

	/** The GL list of each character loaded */
	unsigned int m_gllists;
public:
	FTFont();
	virtual ~FTFont();

	/**
	 * Opens a given filename and read 'charcount' glyphs starting inposition 'startchar'
	 * @param fn the file to load from
	 * @param size the font size
	 * @param startchar the character to start loading from (default: space ' ', 0x20)
	 * @param charcount number of chars to read from the font file
	 */
	bool open(const char* fn, unsigned int size, unsigned int startchar = 32, unsigned int charcount = 96);

	/**
	 * Opens a given font from memory and read 'charcount' glyphs starting inposition 'startchar'
	 * @param buffer the char buffer to load from
	 * @param bufsize the size of the buffer to use
	 * @param size the font size
	 * @param startchar the character to start loading from (default: space ' ', 0x20)
	 * @param charcount number of chars to read from the font file
	 */
	bool open(const char* buffer, unsigned int bufsize, unsigned int size, unsigned int startchar = 32, unsigned int charcount = 96);

	/** Erases the font */
	void release();

	/**
	 * Returns the total width used by a given string
	 */
	float getWidth(const char *str, ...) const;

	virtual void draw(const Rect& rect, const unsigned short* text) const;
	virtual void getSize(const unsigned short* text, int* w, int* h) const;

};

}

#endif /* __SDLENGINE_FONT_H */
