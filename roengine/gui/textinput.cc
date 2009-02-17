#include "stdafx.h"

#include "textinput.h"
#include "gui.h"
#include "font.h"

void GUI::TextInput::setText(const std::string& s) {
	m_text = s;
}
std::string& GUI::TextInput::getText() { return(m_text); }
const std::string& GUI::TextInput::getText() const { return(m_text); }

void GUI::TextInput::Draw() {

}