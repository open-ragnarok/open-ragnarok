#include "stdafx.h"

#include "sdle/font.h"

#include <stdexcept>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

namespace sdle {

Font::Font() {
	m_valid = false;
}

Font::~Font() {
}

bool Font::valid() const {
	return(m_valid);
}

void Font::drawText(float x, float y, const char *str, ...) const {
    if(!valid()) {
        throw std::logic_error("Invalid Font::drawText call.");
    }

    std::va_list args;
    char buf[1024];

    va_start(args,str);
	vsnprintf(buf, 1024, str, args);   // avoid buffer overflow
    va_end(args);

	Rect r;
	r.x = x;
	r.y = y;
	r.w = 0;
	r.h = 0;

	unsigned int buflen = strlen(buf);
	unsigned short *sbuf = new unsigned short[buflen + 1];
	for (unsigned int i = 0; i <= buflen; i++) // Yes, <=. It also copies the ending '0'.
		sbuf[i] = (unsigned short)buf[i];

	draw(r, sbuf);

	delete[] sbuf;
}

void Font::drawText(float x, float y, const unsigned short *str) const {
    if(!valid()) {
        throw std::logic_error("Invalid Font::drawText call.");
    }

	Rect r;
	r.x = x;
	r.y = y;
	r.w = 0;
	r.h = 0;

	draw(r, str);
}

void Font::drawTextBox(float x, float y, float w, float h, const unsigned short *str) const {
    if(!valid()) {
        throw std::logic_error("Invalid Font::drawText call.");
    }

	Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;

	draw(r, str);
}

void Font::drawTextBox(float x, float y, float w, float h, const char *str, ...) const {
    if(!valid()) {
        throw std::logic_error("Invalid Font::drawText call.");
    }

    std::va_list args;
    char buf[1024];

    va_start(args,str);
	vsnprintf(buf, 1024, str, args);   // avoid buffer overflow
    va_end(args);

	Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;

	unsigned int buflen = strlen(buf);
	unsigned short *sbuf = new unsigned short[buflen + 1];
	for (unsigned int i = 0; i <= buflen; i++) // Yes, <=. It also copies the ending '0'.
		sbuf[i] = (unsigned short)buf[i];

	draw(r, sbuf);

	delete[] sbuf;
}

void Font::getSize(const char* text, int* w, int* h) const {
	unsigned short *stext;
	int len = strlen(text);
	stext = new unsigned short[len];

	for (int i = 0; i < len; i++) {
		stext[i] = text[i];
	}
	getSize(stext, w, h);

	delete[] stext;
}

}
