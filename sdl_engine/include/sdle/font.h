#ifndef __SDLENGINE_FONT_H
#define __SDLENGINE_FONT_H

#include "sdle_import.h"

#include <cstdarg>

namespace sdle {

#ifdef UNICODE
	typedef unsigned short Char;
#else
	typedef char Char;
#endif

typedef struct strRect {
	float x, y;
	float w, h;
} Rect;

class SDLENGINE_DLLAPI Font {
protected:
	/** Indicates if the texture is valid for drawing */
	bool m_valid;

public:
	Font();
	virtual ~Font();

	/** Checks the validity of the current font object */
	bool valid() const;

	/** Draws the text in the given xy position */
	void drawText(float x, float y, const char *str, ...) const;

	/** 
	 * Draws the text in the given xy position
	 * @param x
	 * @param y
	 * @param str array of unsigned shorts containing the text to be drawn. Works with unicode strings.
	 */
	void drawText(float x, float y, const unsigned short *str) const;

	/**
	 * Draws the text to fit in a box (the box is not actually drawn. It's just used for clipping.)
	 * Currently param "h" is ignored.
	 */
	void drawTextBox(float x, float y, float w, float h, const char *str, ...) const;

	/** 
	 * Draws the text in the given xy position
	 * @param x
	 * @param y
	 * @param w
	 * @param h height of the text (currently ignored)
	 * @param str array of unsigned shorts containing the text to be drawn. Works with unicode strings.
	 */
	void drawTextBox(float x, float y, float w, float h, const unsigned short *str) const;

	/** 
	 * Function that actually draws the text. Must be overloaded by children classes.
	 * @param rect
	 * @param text unsigned short vector containing the characters (thus, supporting unicode characters)
	 */
	virtual void draw(const Rect& rect, const unsigned short* text) const = 0;

	/**
	 * Retrieves the size (width and height) of the given text
	 */
	virtual void getSize(const unsigned short* text, int* w, int* h) const = 0;

	/**
	 * Retrieves the size (width and height) of the given text
	 */
	void getSize(const char* text, int* w, int* h) const;
};

}

#endif /* __SDLENGINE_FONT_H */
