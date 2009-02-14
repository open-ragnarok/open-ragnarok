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
		} Char;

		typedef struct {
			unsigned int Tex;
			int TexWidth, TexHeight;
			int IntStart, IntEnd;
			Char *Char;
		} Font;

		Font m_font;
	public:
		GLFFont();
		GLFFont(const std::string&);
		~GLFFont();

		bool load(const std::string&);
		virtual void textOut(const std::string&, float x, float y, float z) const;

	};

}

#endif
