/* $Id$ */
#include "stdafx.h"

#include "roengine/gui/textinput.h"
#include "roengine/gui/gui.h"
#include "roengine/gui/font.h"

#include <GL/gl.h>
#include <GL/glu.h>

unsigned int GUI::TextInput::cursor_delay = 500;

GUI::TextInput::TextInput(Element* parent, const TiXmlElement* e, TextureManager& tm, FileManager& fm) : Label(parent, e, tm, fm) {
	m_focusable = true;
	m_delay = 0;
	m_pos = m_text.size();
	m_start = 0;
	m_bar = true;
	actived = false;

	m_MouseDown = FALSE;
	m_charpos = 0;
}

int GUI::TextInput::GetcharposNum(int charpos)
{
	GUI::Gui& gui = GUI::Gui::getSingleton();
	const GUI::Font* font = gui.getDefaultFont();

	unsigned int i;

	for(i = 1 ;i <= m_text.length()-charpos ;i++)
	{
		std::cout << "* i:" << i<< "len"<< font->getWidth( m_text.substr(charpos, i)) << std::endl;
		if( font->getWidth( m_text.substr(charpos, i) ) > MaxLen )
		{
			std::cout << i-1<< std::endl;
			return i-1;
			break;
		}
	}

	return 0;
}

unsigned int GUI::TextInput::GetcharposNum()
{
	GUI::Gui& gui = GUI::Gui::getSingleton();
	const GUI::Font* font = gui.getDefaultFont();

	unsigned int i;

	for(i = 1 ;i <= m_text.length()-m_charpos ;i++)
	{
		std::cout << "i:" << i<< "len"<< font->getWidth( m_text.substr(m_charpos, i)) << std::endl;
		if( font->getWidth( m_text.substr(m_charpos, i) ) > MaxLen )
		{
			std::cout << i-1<< std::endl;
			return i-1;
			break;
		}
	}

	return m_pos+1;
}

void GUI::TextInput::Draw(unsigned int delay) {
	int x = 0;
	int tmp_pos = 0;

	m_delay += delay;
	GUI::Gui& gui = GUI::Gui::getSingleton();
	const GUI::Font* font = gui.getDefaultFont();

	//GUI::Label::Draw(delay);

	
	glColor3f(0.0f,0.0f,0.0f);
	if( MaxLen > 0 )
	{
		unsigned int i;
		for(i = 1 ;i <= m_text.length()-m_charpos ;i++)
		{
			if( font->getWidth( m_text.substr(m_charpos, i) ) > MaxLen )
			{
				break;
			}
		}
		//std::cout << m_text.substr(m_charpos,--i) <<"|||"<< m_text.length() <<"|||" << GetcharposNum() << "|||" << m_pos << std::endl;
		gui.textOut(m_text.substr(m_charpos,--i), (float)pos_x, (float)pos_y, 0,MaxLen);
	}
	else
	{
		gui.textOut(m_text, (float)pos_x, (float)pos_y, 0,MaxLen);
	}
	glColor3f(1.0f,1.0f,1.0f);

	if( MaxLen > 0 )
	{
		x = (int)(font->getWidth( m_text.substr(m_charpos,m_pos) ));
	}
	else
	{
		x = (int)(font->getWidth(m_text.substr(m_start, m_pos - m_start)));
	}

	//int x = (int)(font->getWidth(m_text.substr(m_start, m_pos - m_start)));

	while (m_delay > cursor_delay) {
		m_delay -= cursor_delay;
		m_bar = !m_bar;
	}
	if (m_bar && actived) {
		glDisable(GL_TEXTURE_2D);
		glColor3f(0,0,0);
		glBegin(GL_LINES);
		glVertex3i(x + pos_x, pos_y + 3, 0);
		glVertex3i(x + pos_x, pos_y + 15, 0);
		glEnd();
		glColor3f(1,1,1);
		glEnable(GL_TEXTURE_2D);
	}
}

bool GUI::TextInput::HandleMouseDown(int x, int y, int button) {
	if (!m_enabled)
		return(false);

	std::cout << getName() << "::MouseDown (" << x << ", " << y << ")" << std::endl;

	if (!(button & 1))
		return(false);

	if (m_parent != NULL) {
		if (m_parent->getActiveChild() != NULL)
		{
			//m_parent->getActiveChild()->onLoseFocus();
			m_parent->setActiveChild(this);
			//onGetFocus();
			GUI::Gui& gui = GUI::Gui::getSingleton();
			gui.setFocus(this);
		}
		else
		{
			return(false);
		}
	}
	else
	{
		return(false);
	}

	return(true);
}

void GUI::TextInput::onGetFocus()
{
	actived = true;
}

void GUI::TextInput::onLoseFocus()
{
	actived = false;
}

bool GUI::TextInput::HandleKeyDown(int key, int mod) {
	// Do not handle key input with CTRL, ALT or META.
	int modflag = KMOD_LCTRL | KMOD_RCTRL | KMOD_LALT | KMOD_RALT | KMOD_LMETA | KMOD_RMETA;
	if (mod & modflag) {
		if (m_parent == NULL)
			return(false);
		return(m_parent->HandleKeyDown(key, mod));
	}

	if (key == SDLK_BACKSPACE) {
		if (m_charpos+m_pos > m_text.length())
			m_charpos = m_text.length() - m_pos;
		if (m_charpos+m_pos <= 0)
			return(true);
		m_text.erase(m_charpos+m_pos - 1, 1);
		m_pos--;
		std::cout << "m_pos:"<< m_pos<< std::endl;
		if( m_pos <= 0 )
		{
			if( m_charpos >= 5 )
			{
				if( GetcharposNum(m_charpos-5) == 0 )
				{
					m_pos = 5;
					m_charpos -= 5;
				}
				else if( GetcharposNum(m_charpos-5) >= 5 )
				{
					m_pos = 5;
					m_charpos -= 5;
				}
				else
				{
					m_pos = GetcharposNum(m_charpos-5)-1;
					m_charpos -= GetcharposNum(m_charpos-5)-1;
				}
			}
			else if( m_charpos == 0)
			{
				m_pos = 0;
			}
			else
			{
				if( GetcharposNum(0) == 0 )
				{
					m_pos = m_charpos+1;
					m_charpos = 0;
				}
				else if( GetcharposNum(0) >= m_charpos )
				{
					m_pos = m_charpos+1;
					m_charpos = 0;
				}
				else
				{
					m_pos = GetcharposNum(0)-1;
					m_charpos -= GetcharposNum(0)-1;
				}
			}
		}
		return(true);
	}

	if (key == SDLK_HOME) {
		m_charpos = 0;
		m_pos = 0;
		return(true);
	}

	if (key == SDLK_END) {

		GUI::Gui& gui = GUI::Gui::getSingleton();
		const GUI::Font* font = gui.getDefaultFont();

		int i,len = m_text.length();

		for(i = len-1 ;i >= 0 ;i--)
		{
			if( font->getWidth( &m_text[i] ) > MaxLen )
				break;
		}

		m_charpos = i + 1;
		m_pos = m_text.length() - 1 - i;
		return(true);
	}

	if (key == SDLK_DELETE) {
		if (m_charpos+m_pos == m_text.length())
			return(true);
		m_text.erase(m_charpos+m_pos, 1);
		if( m_pos >= GetcharposNum() )
		{
			if( GetcharposNum() > 5 )
			{
				if( m_text.length() - m_charpos - GetcharposNum() - 5 >= 0 )
				{
					m_charpos += 5;
					m_pos -= 5;
				}
				else
				{
					int Num = m_text.length() - m_charpos - GetcharposNum();
					m_charpos += Num;
					m_pos -= Num;
				}
			}
			else
			{
				if( m_text.length() - m_charpos - GetcharposNum() - 5 >= GetcharposNum()-1 )
				{
					m_charpos += GetcharposNum()-1;
					m_pos -= GetcharposNum()-1;
				}
				else
				{
					int Num = m_text.length() - m_charpos - GetcharposNum();
					m_charpos += Num;
					m_pos -= Num;
				}
			}
		}
		return(true);
	}

	if (key == SDLK_LEFT) {
		if(m_charpos+m_pos == 0)
			return(true);
		m_pos--;
		if( m_pos <= 0 )
		{
			if( m_charpos >= 5 )
			{
				if( GetcharposNum(m_charpos-5) == 0 )
				{
					m_pos = 5;
					m_charpos -= 5;
				}
				else if( GetcharposNum(m_charpos-5) >= 5 )
				{
					m_pos = 5;
					m_charpos -= 5;
				}
				else
				{
					m_pos = GetcharposNum(m_charpos-5)-1;
					m_charpos -= GetcharposNum(m_charpos-5)-1;
				}
			}
			else if( m_charpos == 0)
			{
				m_pos = 0;
				m_charpos = 0;
			}
			else
			{
				if( GetcharposNum(0) == 0 )
				{
					m_pos = m_charpos+1;
					m_charpos = 0;
				}
				else if( GetcharposNum(0) >= m_charpos )
				{
					m_pos = m_charpos+1;
					m_charpos = 0;
				}
				else
				{
					m_pos = GetcharposNum(0)-1;
					m_charpos -= GetcharposNum(0)-1;
				}
			}
		}
	}

	if (key == SDLK_RIGHT) {
		m_pos++;
		if (m_charpos+m_pos > m_text.length())
		{
			m_pos--;
		}

		if( m_pos > GetcharposNum() )
		{
			if( GetcharposNum() > 5 )
			{
				if( m_text.length() - m_charpos - GetcharposNum() - 5 >= 0 )
				{
					m_charpos += 5;
					m_pos -= 5;
				}
				else
				{
					int Num = m_text.length() - m_charpos - GetcharposNum();
					m_charpos += Num;
					m_pos -= Num;
				}
			}
			else
			{
				if( m_text.length() - m_charpos - GetcharposNum() - 5 >= GetcharposNum()-1 )
				{
					m_charpos += GetcharposNum()-1;
					m_pos -= GetcharposNum()-1;
				}
				else
				{
					int Num = m_text.length() - m_charpos - GetcharposNum();
					m_charpos += Num;
					m_pos -= Num;
				}
			}
		}
	}

	if ((key >= SDLK_a && key <= SDLK_z) || (key == SDLK_SPACE)) {
		char buf[2];
		if (mod & (KMOD_LSHIFT | KMOD_RSHIFT))
			sprintf(buf, "%c", key + 'A' - 'a');
		else
			sprintf(buf, "%c", key);

		if ( m_charpos+m_pos >= m_text.length()) {
			m_text += buf;
			m_pos++;
			std::cout << "m_pos:" << m_pos <<  "m_charpos:" << m_charpos <<std::endl;
			if( MaxLen > 0 && m_pos >= GetcharposNum() )
			{
				while(true)
				{
					m_charpos++;
					m_pos --;
					if( GetcharposNum(m_charpos) == 0 )
						break;
				}
			}
		}
		else {
			//sprintf(buf, "%c", key);
			m_text.insert(m_charpos+m_pos, buf);
			m_pos++;
			if( MaxLen > 0 && m_pos >= GetcharposNum() )
			{
				if( GetcharposNum() > 5 )
				{
					if( m_text.length() - m_charpos - GetcharposNum() - 5 >= 0 )
					{
						m_charpos += 5;
						m_pos -= 5;
					}
					else
					{
						int Num = m_text.length() - m_charpos - GetcharposNum();
						m_charpos += Num;
						m_pos -= Num;
					}
				}
				else
				{
					if( m_text.length() - m_charpos - GetcharposNum() - 5 >= GetcharposNum()-1 )
					{
						m_charpos += GetcharposNum()-1;
						m_pos -= GetcharposNum()-1;
					}
					else
					{
						int Num = m_text.length() - m_charpos - GetcharposNum();
						m_charpos += Num;
						m_pos -= Num;
					}
				}
			}
		}
		return(true);
	}

	if ((key >= SDLK_0 && key <= SDLK_9) || (key >= SDLK_KP0 && key <= SDLK_KP9)) {
		char buf[2];
		if (mod & (KMOD_LSHIFT | KMOD_RSHIFT))
			sprintf(buf, "%c", key);//sprintf(buf, "%c", key + 'A' - 'a');
		else
			sprintf(buf, "%c", key);

		if (m_charpos+m_pos >= m_text.length()) {
			m_text += buf;
			m_pos++;
			std::cout << "m_pos:" << m_pos <<  "m_charpos:" << m_charpos <<std::endl;
			if( MaxLen > 0 && m_pos >= GetcharposNum() )
			{
				while(true)
				{
					m_charpos++;
					m_pos --;
					if( GetcharposNum(m_charpos) == 0 )
						break;
				}
			}
		}
		else {
			m_text.insert(m_charpos+m_pos, buf);
			m_pos++;
			if( MaxLen > 0 && m_pos >= GetcharposNum() )
			{
				if( GetcharposNum() > 5 )
				{
					if( m_text.length() - m_charpos - GetcharposNum() - 5 >= 0 )
					{
						m_charpos += 5;
						m_pos -= 5;
					}
					else
					{
						int Num = m_text.length() - m_charpos - GetcharposNum();
						m_charpos += Num;
						m_pos -= Num;
					}
				}
				else
				{
					if( m_text.length() - m_charpos - GetcharposNum() - 5 >= GetcharposNum()-1 )
					{
						m_charpos += GetcharposNum()-1;
						m_pos -= GetcharposNum()-1;
					}
					else
					{
						int Num = m_text.length() - m_charpos - GetcharposNum();
						m_charpos += Num;
						m_pos -= Num;
					}
				}
			}
		}
		return(true);
	}

	if (m_parent == NULL)
		return(false);
	return(m_parent->HandleKeyDown(key, mod));
}
