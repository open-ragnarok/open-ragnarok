/* $Id$ */
#include "stdafx.h"

#include "window.h"

GUI::Window::Window() : Element() {
}

GUI::Window::Window(const Texture::Pointer& t) : Element() {
	texture = t;
}

void GUI::Window::setCaption(const std::string& n) {
	caption = n;
}

GUI::Window::Window(Element* parent, const TiXmlElement* node, TextureManager& tm, FileManager& fm) : Element(parent, node, tm, fm) {

}
