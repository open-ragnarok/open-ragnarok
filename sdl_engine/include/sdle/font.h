#ifndef __SDLENGINE_FONT_H
#define __SDLENGINE_FONT_H

#include "sdle_import.h"

#include <cstdarg>

namespace sdle {

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
	 * Draws the text to fit in a box (the box is not actually drawn. It's just used for clipping.)
	 * Currently param "h" is ignored.
	 */
	void drawTextBox(float x, float y, float w, float h, const char *str, ...) const;

	/** 
	 * Function that actually draws the text. Must be overloaded by children classes.
	 */
	virtual void draw(const Rect& rect, const char* text) const = 0;
};

}

#endif /* __SDLENGINE_FONT_H */
