/* $Id$ */
#ifndef __SDLENGINE_GLF_FONT_H
#define __SDLENGINE_GLF_FONT_H

#include "font.h"
#include "texture.h"

namespace sdle {

/** Manages fonts in the old GLF format */
class SDLENGINE_DLLAPI GLFFont : public Font {
protected:
	typedef struct 	{
		float dx, dy;
		float tx1, ty1;
		float tx2, ty2;
	} _Char;

	typedef struct {
		unsigned int Tex;
		int TexWidth, TexHeight;
		int IntStart, IntEnd;
		_Char *Char;
	} FontInfo;

	FontInfo m_font;
	float fontmult;

	/** Texture holder */
	Texture m_texture;
public:
	GLFFont();
	
	/** Creates a GLF Font from data in the memory */
	GLFFont(const unsigned char*, unsigned int);
	virtual ~GLFFont();

	/** Releases all the data */
	void release();

	bool load(std::istream&);
	bool load(const std::string&);
	virtual void textOut(const std::string&, float x, float y, float z, int MaxLen) const;
	virtual float getWidth(const std::string&) const;

	virtual void draw(const Rect& rect, const char* text) const;
};

extern const GLFFont Font_Arabia8;
extern const GLFFont Font_Arial8;
extern const GLFFont Font_Arial10;

}

#endif /* __SDLENGINE_GLF_FONT_H */
