/* $Id$ */
#include "stdafx.h"

#include "roengine/gui/textinput.h"
#include "roengine/gui/gui.h"
#include "sdle/ft_font.h"

#include <GL/gl.h>
#include <GL/glu.h>

unsigned int GUI::TextInput::cursor_delay = 500;

GUI::TextInput::~TextInput() {
}

GUI::TextInput::TextInput(Element* parent, const TiXmlElement* e, TextureManager& tm, FileManager& fm) : Label(parent, e, tm, fm) {
	m_focusable = true;
	m_delay = 0;
	m_pos = 0;
	m_start = 0;
	m_bar = true;
	actived = false;
	SelectFlag = false;

	m_MouseDown = FALSE;

	G_Text.Surface  = SDL_GetVideoSurface();
	G_Text.Text_Changed	= false;
	if( MaxLen <= 0 ) {
		MaxLen = 0;
	}
	G_Text.Len		= MaxLen;
	G_Text.x        = pos_x;
	G_Text.y        = pos_y + 5;	// By Naxan
	G_Text.z        = 0;
	G_Text.EffectWidth = w;
	G_Text.FColor   = SDL_BLACK;
	G_Text.BColor   = SDL_WHITE;
	G_Text.SFColor   = SDL_WHITE;
	G_Text.SBColor   = SDL_BLUE;
	G_Text.Alpha    = SDL_OPACITY;
	G_Text.m_text   = (LCHAR*)calloc(MaxLen+sizeof(LCHAR), sizeof(LCHAR));

	// TODO: put this in the FontManager
	//sdle::FTFont *font = new sdle::FTFont();
	//G_Text.Font = font;
	//font->open("data\\font\\lsans.ttf", 12);

	G_Text.Head = G_Text.Start  = G_Text.Current = G_Text.select1 = G_Text.select2 = NULL;// G_Text.End = NewNode(NULL, NULL, AU('\0'));
}

void GUI::TextInput::Draw(unsigned int delay) {
	int x = 0;
	int tmp_pos = 0;

	GUI::Gui& gui = GUI::Gui::getSingleton();

	m_delay += delay;

	//GetStringFromNode();
	gui.TextOutEx(&G_Text);

	x = GetCursorX();

	while (m_delay > cursor_delay) {
		m_delay -= cursor_delay;
		m_bar = !m_bar;
	}
	if (m_bar && actived) {
		glDisable(GL_TEXTURE_2D);
		glColor3f(0,0,0);
		glBegin(GL_LINES);
		glVertex3i(x+1, pos_y + 3, 0);
		glVertex3i(x+1, pos_y + 15, 0);
		glEnd();
		glColor3f(1,1,1);
		glEnable(GL_TEXTURE_2D);
	}

	return;

/*
	if (G_Text.Text_Changed) {
		ShowCursor(true);
	}

	return;

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
*/
}

bool GUI::TextInput::HandleMouseRelease(int x, int y, int button) {
	SelectFlag = false;
	return(false);
}

bool GUI::TextInput::HandleMouseDown(int x, int y, int button) {
	if (!m_enabled)
		return(false);

	std::cout << getName() << "::MouseDown (" << x << ", " << y << ")" << std::endl;

	if (!(button & 1))
		return(false);
	SelectFlag = true;

	ClickPos(x);

	if (m_parent != NULL) {
		if (m_parent->getActiveChild() != NULL) {
			//m_parent->getActiveChild()->onLoseFocus();
			m_parent->setActiveChild(this);
			//onGetFocus();
			GUI::Gui& gui = GUI::Gui::getSingleton();
			gui.setFocus(this);
		}
		else {
			return(false);
		}
	}
	else {
		return(false);
	}

	return(true);
}

void GUI::TextInput::onGetFocus() {
	actived = true;
}

void GUI::TextInput::onLoseFocus() {
	actived = false;
}

bool GUI::TextInput::HandleKeyUp(SDL_Event *sdlEvent, int mod) {

	SDLKey key = sdlEvent->key.keysym.sym;

	if(key == SDLK_RSHIFT)
		SelectFlag = false;
	else if(key == SDLK_LSHIFT)
		SelectFlag = false;
	if (m_parent == NULL)
		return(false);
	return(m_parent->HandleKeyUp(sdlEvent, mod));
}

bool GUI::TextInput::HandleKeyDown(SDL_Event *sdlEvent, int mod) {
	SDLKey key = sdlEvent->key.keysym.sym;

	if(key == SDLK_BACKSPACE)
		RemoveLeft();
	else if(key == SDLK_DELETE)
		RemoveRight();
	else if(key == SDLK_LEFT)            
		MoveLeft();
	else if(key == SDLK_RIGHT)
		MoveRight();
	else if(key == SDLK_HOME)
		MoveStart();
	else if(key == SDLK_END)
		MoveEnd();
	else if(mod & KMOD_LSHIFT)
		SelectFlag = true;
	else if(mod & KMOD_RSHIFT)
		SelectFlag = true;
	else if( (mod & KMOD_LCTRL) && (key == 97 || key == 65 ) )
		SelectAll();
	else if( (mod & KMOD_RCTRL) && (key == 97 || key == 65 ) )
		SelectAll();
	else if(sdlEvent->key.keysym.unicode != 0)
		Insert(sdlEvent->key.keysym.unicode);
	else
		Insert(sdlEvent->key.keysym.sym);

	printf("Text input:\t");
	printf("Unicode: %d\t", sdlEvent->key.keysym.unicode);
	printf("Sym: %d", sdlEvent->key.keysym.sym);
	printf("Char: [%c]", sdlEvent->key.keysym.sym);
	printf("\n");


	if (m_parent == NULL)
		return(false);
	return(m_parent->HandleKeyDown(sdlEvent, mod));
}

//==================================================================================
// 从结构key.keysym.unicode插入字符
//==================================================================================
bool GUI::TextInput::Insert(Uint16 Ch) {
	NODE *tmp;
	if (G_Text.Len <= 0)
		return false;

	if (G_Text.Head == NULL) {
		G_Text.Head = G_Text.Start  = G_Text.Current = NewNode(NULL, NULL, Ch);
		G_Text.Len--;
		return true;
	}

	if (Ch <= 31) {
		return false;
	}

	if (G_Text.Current != NULL) {
		if (G_Text.Current->Next != NULL) {
			tmp = G_Text.Current->Next;
			G_Text.Current->Next = NewNode(G_Text.Current, tmp, Ch);
			if (G_Text.Current->Next == NULL) {
				G_Text.Current->Next = tmp;
				return false;
			}

			tmp->Prev = G_Text.Current->Next;
			if (G_Text.Current->Next != NULL) {
				G_Text.Current = G_Text.Current->Next;
			}
		}
		else {
			G_Text.Current->Next = NewNode(G_Text.Current, NULL, Ch);
			if (G_Text.Current->Next != NULL) {
				G_Text.Current = G_Text.Current->Next;
			}
			else {
				return false;
			}
		}
	}
	else {
		tmp = NewNode(NULL, G_Text.Start, Ch);
		if (tmp != NULL) {
			G_Text.Start->Prev = tmp;
			G_Text.Head = G_Text.Start = G_Text.Current = tmp;
		}
		else {
			return false;
		}
	}

	CheckPos();

	G_Text.Len--;
	return true;
}
//==================================================================================
// 新的节点
//==================================================================================
NODE* GUI::TextInput::NewNode(OUT NODE *Front, OUT NODE *Behind, IN LCHAR Ch) {
	NODE *Tmp = (NODE*)malloc(sizeof(NODE));
	if (Tmp != NULL) {
		Tmp->Ch   = Ch;
		Tmp->selected   = false;
		Tmp->Prev = Front;
		Tmp->Next = Behind;
		Tmp->Font_Screen = NULL;

		G_Text.Text_Changed = true;
	}
	return Tmp;
}
//==================================================================================
// 从节点获取字符保存成串
//==================================================================================
void GUI::TextInput::GetStringFromNode(void) {       
	NODE *i;
	int  j=0, w, h;

	const sdle::Font* font = GUI::Gui::getSingleton().getDefaultFont();
	if (font == NULL)
		return;

	if (G_Text.Start == NULL) {
		G_Text.m_text[j] = AU('\0');
		return;
	}
	for (i = G_Text.Start; i != NULL; i = i->Next) {
		G_Text.m_text[j++] = i->Ch;
		G_Text.m_text[j] = AU('\0');
		font->getSize(G_Text.m_text, &w, &h);
		if (w > this->w) {
			G_Text.m_text[j-1] = AU('\0');
			break;
		}

	}

	G_Text.m_text[j] = AU('\0');
}
//==================================================================================
// 删除左边一个字符
//==================================================================================
bool GUI::TextInput::RemoveLeft(void) {
	NODE *tmp;

	if( G_Text.Start == NULL || G_Text.Current == NULL ) {
		return false;
	}

	if (G_Text.Current->Prev != NULL) {
		if (G_Text.Current->Prev->Prev != NULL) {
			tmp = G_Text.Current;
			G_Text.Current = tmp->Prev;
			G_Text.Current->Next = tmp->Next;
			if (tmp->Next != NULL) {
				tmp->Next->Prev = G_Text.Current;
			}

			free(tmp);
		}
		else {
			tmp = G_Text.Current;
			G_Text.Current = tmp->Prev;
			G_Text.Current->Next = tmp->Next;
			//G_Text.Start = G_Text.Current;

			free(tmp);
		}
	}
	else {
		tmp = G_Text.Current;
		G_Text.Current = NULL;
		G_Text.Head = G_Text.Start = tmp->Next;
		if (G_Text.Start != NULL) {
			G_Text.Start->Prev = NULL;
		}
		free(tmp);
	}

	CheckPos();
	++G_Text.Len;
	G_Text.Text_Changed = true;
	return true;
}
//==================================================================================
// 删除右边一个字符
//==================================================================================
bool GUI::TextInput::RemoveRight(void) {       
	NODE *tmp;

	if (G_Text.Start == NULL) {
		return false;
	}

	if (G_Text.Current != NULL) {
		if (G_Text.Current->Next != NULL) {
			if (G_Text.Current->Next->Next != NULL) {
				tmp = G_Text.Current->Next;
				G_Text.Current->Next = tmp->Next;
				tmp->Next->Prev = G_Text.Current;

				free(tmp);
			}
			else {
				tmp = G_Text.Current->Next;
				G_Text.Current->Next = NULL;

				free(tmp);
			}
		}
		else {
			return false;
		}
	}
	else {
		if (G_Text.Start->Next != NULL) {
			tmp = G_Text.Start->Next;
			G_Text.Head = G_Text.Start = tmp;
			tmp = G_Text.Start->Prev;
			G_Text.Start->Prev = NULL;

			free(tmp);
		}
		else {
			free(G_Text.Start);
			G_Text.Head = G_Text.Start = NULL;
		}
	}

	CheckPos();
	++G_Text.Len;
	G_Text.Text_Changed = true;
	return true;
}
//==================================================================================
// 左移动一个字符
//==================================================================================
bool GUI::TextInput::MoveLeft(void) {
	if (G_Text.Start == NULL || G_Text.Current == NULL) {
		return false;
	}
	G_Text.Text_Changed = false;

	if (G_Text.Current->Prev != NULL) {
		G_Text.Current = G_Text.Current->Prev;
	}
	else {
		G_Text.Current = NULL;
	}

	CheckPos();

	return true;
}
//==================================================================================
// 右移动一个字符
//==================================================================================
bool GUI::TextInput::MoveRight(void) {
	if (G_Text.Start == NULL) {
		return false;
	}

	G_Text.Text_Changed = false;

	if (G_Text.Current == NULL) {
		G_Text.Current = G_Text.Start;
	}
	else {
		if (G_Text.Current->Next != NULL) {
			G_Text.Current = G_Text.Current->Next;
		}
		else {
			return false;
		}
	}

	CheckPos();

	return true;
}
//==================================================================================
// 移动到字符串开始
//==================================================================================
bool GUI::TextInput::MoveStart(void) {
	G_Text.Start = G_Text.Head;
	G_Text.Current = NULL;
	return true;
}
//==================================================================================
// 移动到字符串尾
//==================================================================================
bool GUI::TextInput::MoveEnd(void) {
	while( MoveRight() )
		;
	return true;
}
//==================================================================================
// 释放字符串
//==================================================================================
void GUI::TextInput::FreeString(void) {
	NODE *Tmp;

	G_Text.Start = G_Text.Head;
	while (G_Text.Start != NULL) {
		Tmp = G_Text.Start->Next;
		free(G_Text.Start);
		G_Text.Start = Tmp;
	}
	free(G_Text.m_text);
}
//==================================================================================
// 获取鼠标X坐标
//==================================================================================
int GUI::TextInput::GetCursorX() {
	NODE *i;
	int k = 0, w, h;
	LCHAR *Buffer = NULL;

	Buffer = new LCHAR[MaxLen+1];

	const sdle::Font* font = GUI::Gui::getSingleton().getDefaultFont();
	if (font == NULL)
		return(0);


	if (Buffer == NULL)
		return 0;

	if (G_Text.Start != NULL && G_Text.Current != NULL) {
		for (i = G_Text.Start; i != G_Text.Current->Next; i = i->Next) {
			Buffer[k++] = i->Ch;
			Buffer[k] = AU('\0');
			font->getSize(Buffer, &w, &h);
			if (w > this->w) {
				k--;
				break;
			}
		}
	}
	Buffer[k] = AU('\0');
	font->getSize(Buffer, &w, &h);
	delete []Buffer;
	return (G_Text.x + w);
}
//==================================================================================
// 判断左右极限
//==================================================================================
bool GUI::TextInput::CheckPos() {
	int Step = 3;
	NODE *i;
	int k = 0, w, h;
	LCHAR *Buffer = NULL;

	const sdle::Font* font = GUI::Gui::getSingleton().getDefaultFont();
	if (font == NULL)
		return(false);

	if (G_Text.Start == NULL || G_Text.Current == NULL) {
		return false;
	}

	if (G_Text.Current == G_Text.Start) {
		for(int i = 0; i < Step; i++) {
			if (G_Text.Start->Prev == NULL) {
				break;
			}
			else {
				G_Text.Start = G_Text.Start->Prev;
			}
		}
		return true;
	}

	Buffer = new LCHAR[MaxLen+1];

	if (Buffer == NULL)
		return false;

	for (i = G_Text.Start; i != NULL; i = i->Next) {
		Buffer[k++] = i->Ch;
		if(i == G_Text.Current->Next)
			break;
	}

	Buffer[k] = AU('\0');
	font->getSize(Buffer, &w, &h);
	delete []Buffer;
	if (w > this->w) {
		if (G_Text.Current->Next == NULL) {
			Step = 2;
		}

		for(int i = 0; i < Step ;i++) {
			if (G_Text.Start->Next != NULL)
				G_Text.Start = G_Text.Start->Next;
			else
				break;
		}
		return true;
	}

	return false;
}
//==================================================================================
// 获取鼠标点击
//==================================================================================
void GUI::TextInput::ClickPos(int x) {
	NODE *i;
	int k = 0, w, h;
	LCHAR *Buffer = NULL;

	const sdle::Font* font = GUI::Gui::getSingleton().getDefaultFont();
	if (font == NULL)
		return;

	if (G_Text.Start == NULL) {
		return;
	}

	Buffer = new LCHAR[MaxLen+1];

	if (Buffer == NULL)
		return;

	for (i = G_Text.Start; i != NULL; i=i->Next) {
		Buffer[k++] = i->Ch;
		Buffer[k] = AU('\0');
		font->getSize(Buffer, &w, &h);
		if (w > this->w) {
			G_Text.Current = i->Prev;
			break;
		}
		else if (w > x) {
			G_Text.Current = i->Prev;
			break;
		}
		else {
			G_Text.Current = i;
		}
	}

	delete []Buffer;
	CheckPos();
}

std::string& GUI::TextInput::getText() {
	NODE *i;
	int  j=0;
	LCHAR *Buffer = NULL;

	m_text.clear();

	Buffer = new LCHAR[MaxLen+1];

	if (Buffer == NULL) {
		return m_text;
	}
	if (G_Text.Start == NULL) {
		return m_text;
	}
	for (i = G_Text.Start; i != NULL; i = i->Next) {
		G_Text.m_text[j++] = i->Ch;
	}

	G_Text.m_text[j] = AU('\0');

	DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)G_Text.m_text,-1,NULL,0,NULL,FALSE);
	char *psText;
	psText = new char[dwNum];
	if(!psText) {
		delete []Buffer;
		return m_text;
	}
	WideCharToMultiByte (CP_OEMCP,NULL,(LPCWSTR)G_Text.m_text,-1,psText,dwNum,NULL,FALSE);
	m_text = psText;
	delete []psText;
	delete []Buffer;

	return m_text;
}

void GUI::TextInput::SelectAll() {
	NODE *i;

	MoveEnd();
	G_Text.select1 = G_Text.Head;
	G_Text.select2 = G_Text.Current;
	i = G_Text.Head;
	while (i != NULL)
	{
		i->selected = true;
		i = i->Next;
	}
}

void GUI::TextInput::UnSelectAll() {
	NODE *i;

	i = G_Text.Head;
	while (i != NULL) {
		i->selected = false;
		i = i->Next;
	}
}
