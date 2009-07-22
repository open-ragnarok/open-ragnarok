/* $Id$ */
#ifndef __GUI_GLF_FONT_H
#define __GUI_GLF_FONT_H

#include "font.h"

namespace GUI {
	class GLFFont {
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
		} Font;

		Font m_font;
		float fontmult;
	public:
		GLFFont();
		GLFFont(const std::string&);
		virtual ~GLFFont();

		bool load(std::istream&);
		bool load(const std::string&);
		virtual void textOut(const std::string&, float x, float y, float z) const;
		virtual float getWidth(const std::string&) const;
	};

}

#endif
