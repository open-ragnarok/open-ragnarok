/* $Id$ */
#include "stdafx.h"

#include "roengine/gui/gui.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "sdle/glf_font.h"
#include "sdle/ft_font.h"

namespace GUI {

/**
 * Starts OpenGL 2D Mode
 * @param width int the window width
 * @param height int the window height
 */
void Mode2DStart(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, width, height, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
}

/**
 * Ends OpenGL 2D Mode and resets the matrix back to the state
 * when Mode2DStart() was called.
 */
void Mode2DEnd() {
	glEnable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Gui::Init(int w, int h) {
	setSize(w, h);

	m_defaultFont = (sdle::Font*)sdle::Font_Arial10;
}

Gui::Gui() : Singleton<Gui>() {
	m_desktop = NULL;
	m_inactiveDesktop = NULL;
	active = NULL;
	m_defaultFont = NULL;
	m_msgbox_bg = "win_msgbox.bmp";
	m_msgbox_btnok = "login_interface\\btn_ok.bmp";
	m_msgbox_btncancel = "login_interface\\btn_cancel.bmp";
	m_msgbox_btnexit = "login_interface\\btn_exit.bmp";
}

Gui::~Gui() {
	clear();
}

std::string Gui::getMsgboxBg() {
	std::string aux = "texture\\";
	aux += RO::EUC::user_interface;
	aux += "\\";
	aux += m_msgbox_bg;
	return(aux);
}

std::string Gui::getMsgboxOk() {
	std::string aux = "texture\\";
	aux += RO::EUC::user_interface;
	aux += "\\";
	aux += m_msgbox_btnok;
	return(aux);
}

std::string Gui::getMsgboxCancel() {
	std::string aux = "texture\\";
	aux += RO::EUC::user_interface;
	aux += "\\";
	aux += m_msgbox_btncancel;
	return(aux);
}

std::string Gui::getMsgboxExit() {
	std::string aux = "texture\\";
	aux += RO::EUC::user_interface;
	aux += "\\";
	aux += m_msgbox_btnexit;
	return(aux);
}

void Gui::clear() {
	GUI::Element::getCache().clear();
}

void Gui::Draw(unsigned int delay, Vector3f CameraLook) {
	if (!m_desktop && !m_inactiveDesktop)
		return;

	Mode2DStart(m_width, m_height);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.45f);

	if (m_inactiveDesktop) {
		if (!m_desktop->isVisible()) {
			delete(m_desktop);
			m_desktop = m_inactiveDesktop;
			m_inactiveDesktop = NULL;
		}
		else {
			m_inactiveDesktop->Draw(delay);
		}
	}

	if (m_desktop)
		m_desktop->Draw(delay);

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
	Mode2DEnd();
}

void Gui::textOut(const std::string& text, float x, float y, float z,int MaxLen) {
	// TODO: Create a MaxLen method on sdle::Font
	m_defaultFont->drawText(x, y, text.c_str());
}

void Gui::setSize(int w, int h) {
	m_width = w;
	m_height = h;
}

Desktop* Gui::getDesktop() {
	return(m_desktop);
}

const Desktop* Gui::getDesktop() const {
	return(m_desktop);
}

void Gui::setDesktop(Desktop* e) {
	if (m_desktop == e)
		return;
	m_desktop = e;
	if (m_desktop == NULL)
		return;

	active = (Element*)m_desktop;
	while (active->getActiveChild() != NULL) {
		active = active->getActiveChild();
	}
}

bool Gui::setDesktop(const std::string& ui) {
	if (ui == "") {
		setDesktop(NULL);
		return(true);
	}
	Element* e = GUI::Element::getElement(ui);
	if (e == NULL) {
		std::cerr << "No GUI element named " << ui << " found." << std::endl;
		return(false);
	}
	setDesktop((Desktop*)e);
	return(true);
}

BaseCache<sdle::Font>& Gui::FontManager() {
	return(m_fonts);
}

const BaseCache<sdle::Font>& Gui::FontManager() const {
	return(m_fonts);
}

const sdle::Font* Gui::getDefaultFont() {
	return(m_defaultFont);
}

int Gui::getWidth() const {
	return(m_width);
}

int Gui::getHeight() const {
	return(m_height);
}

Element* Gui::getActiveElement() {
	if (active == NULL) {
		if (m_desktop != NULL) {
			Element* next;
			active = m_desktop;
			next = active->getActiveChild();
			while (next != NULL) {
				active = next;
				next = next->getActiveChild();
			}
		}
	}
	return(active);
}

bool Gui::InjectKeyPress(SDL_Event *sdlEvent, const int& mod) {
	Element* e = getActiveElement();
	if (e == NULL)
		return(false);
	return(e->HandleKeyDown(sdlEvent, mod));
}

bool Gui::InjectKeyRelease(SDL_Event *sdlEvent, const int& mod) {
	Element* e = getActiveElement();
	if (e == NULL)
		return(false);
	return(e->HandleKeyUp(sdlEvent, mod));
}

bool Gui::InjectMouseClick(int x, int y, int buttons, int modifier)  {
	Element* e = getDesktop();
	if (e == NULL)
		return(false);
	return(e->HandleMouseDown(x, y, buttons));
}

bool Gui::InjectMouseRelease(int x, int y, int buttons, int modifier)  {
	Element* e = getDesktop();
	if (e == NULL)
		return(false);
	return(e->HandleMouseRelease(x, y, buttons));
}

bool Gui::InjectMouseMove(const int& x, const int& y, const int& dx, const int& dy) {
	Element* e = getDesktop();
	if (e == NULL)
		return(false);
	return(e->HandleMouseMove(x, y, dx, dy));
}

void Gui::setFocus(Element* e) {
	if (active != e) {
		if (active != NULL)
			active->onLoseFocus();
		active = e;
		active->setActive();
		if (active != NULL)
			active->onGetFocus();
	}
}

Element* Gui::operator[] (const std::string& n) {
	return(Element::getElement(n));
}

const Element* Gui::operator[] (const std::string& n) const {
	return(Element::getElement(n));
}

void Gui::PushEvent(const Event& e) {
	m_events.push_back(e);
}

Event Gui::PopEvent() {
	std::vector<Event>::iterator itr;
	itr = m_events.begin();
	if (itr == m_events.end())
		return(Event());
	Event ret = *itr;
	m_events.erase(itr);
	return(ret);
}

int Gui::GetEventCount() const {
	return(m_events.size());
}

void Gui::ProcessEvents() {
	if (m_desktop == NULL)
		return;

	while (GetEventCount())
		m_desktop->HandleEvent(PopEvent());
}

void Gui::Dialog(const std::string& title, const std::string& text, TextureManager& tm, FileManager& fm, int buttons) {
	GUI::Dialog* dialog;
	dialog = new GUI::Dialog(title, text, tm, fm);
	m_inactiveDesktop = m_desktop;
	m_desktop = dialog;
	dialog->setVisible(true);
}

SDL_Color GetRGB(IN SDL_Surface *Surface, IN Uint32 Color){
	SDL_Color Rgb;
	SDL_GetRGB(Color, Surface->format, &(Rgb.r), &(Rgb.g), &(Rgb.b));
	return Rgb;
}

void Gui::ClearFont_Screen(TextEditor *G_Text) {
	NODE *i;

	for (i=G_Text->Start; i != NULL ; i=i->Next) {
		if( i->Font_Screen != NULL ) {
			SDL_FreeSurface(i->Font_Screen);
			i->Font_Screen = NULL;
		}
	}
}

inline int next_p2(int a){
	int rval=1;
	while(rval<a) rval<<=1;
	return rval;
}


int Gui::TextOutEx(TextEditor *G_Text) {
#if 1
	if (G_Text->Start == NULL)
		return(0);
	// BIG hack to make SDLEngine and FreeType work for now
	unsigned short text[128];
	unsigned int i = 0;
	NODE* n = G_Text->Start;
	while (n != NULL) {
		text[i] = n->Ch;
		i++;
		text[i] = 0;
		n = n->Next;
	}

	glColor3f(0, 0, 0);
	G_Text->Font->drawText(G_Text->x, G_Text->y, text);
	glColor3f(1, 1, 1);
#else
	//SDL_Surface *Font_Screen = NULL;
	SDL_Color Font_FColor, Font_BColor, Font_SBColor, Font_SFColor;

	NODE *i;
	int  j=0, w, h,FontW,FontH;
	SDL_Rect rect ={0};
	LCHAR Ch[2] = {0};

	if( G_Text->Start == NULL ) {
		return 0;
	}

	Font_FColor = GetRGB(SDL_GetVideoSurface(), G_Text->FColor);
	Font_BColor = GetRGB(SDL_GetVideoSurface(), G_Text->BColor);
	Font_SFColor = GetRGB(SDL_GetVideoSurface(), G_Text->SFColor);
	Font_SBColor = GetRGB(SDL_GetVideoSurface(), G_Text->SBColor);

	for (i=G_Text->Start; i != NULL ; i=i->Next) {
		G_Text->m_text[j++] = i->Ch;
		G_Text->m_text[j] = AU('\0');
		TTF_SizeUNICODE(G_Text->Font, G_Text->m_text, &w, &h);
		FontH = h;
		Ch[0] = i->Ch;
		if ( w <= G_Text->EffectWidth ) {
			FontW = w;
			if( i->selected ) {
				/* 输出字体 */
				i->Font_Screen = TTF_RenderUNICODE_Shaded(G_Text->Font, Ch, Font_SFColor, Font_SBColor);
			}
			else {
				/* 输出字体 */
				i->Font_Screen = TTF_RenderUNICODE_Shaded(G_Text->Font, Ch, Font_FColor, Font_BColor);
				SDL_SetColorKey(i->Font_Screen, SDL_SRCCOLORKEY, SDL_MapRGB(i->Font_Screen->format, Font_BColor.r, Font_BColor.g, Font_BColor.b));
				if (G_Text->Alpha != SDL_ALPHA_OPAQUE)
				{
					SDL_SetAlpha(i->Font_Screen, SDL_SRCALPHA, G_Text->Alpha);
				}
			}

			if (i->Font_Screen == NULL) {
				std::cout << "::CreateFont failed: " << i->Ch << std::endl;
				ClearFont_Screen(G_Text);
				return -1;
			}
		}
		else {
			G_Text->m_text[j-1] = AU('\0');
			break;
		}

	}

	GLuint hudTextureID;
	SDL_Surface  *tmp = NULL;
	int nWidthPowerOfTwo = next_p2(FontW);
	int nHeightPowerOfTwo = next_p2(FontH);

	tmp = SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_SRCCOLORKEY, nWidthPowerOfTwo, nHeightPowerOfTwo, 32, 0xff, 0xff00, 0xff0000, 0xFF000000);

	if (!tmp)
	{
		ClearFont_Screen(G_Text);
		return -1;
	}

	for (i=G_Text->Start; i != NULL ; i=i->Next)
	{
		if( i->Font_Screen == NULL )
		{
			break;
		}

		rect.x += rect.w;
		rect.y = 0;
		rect.w = i->Font_Screen->w;
		rect.h = i->Font_Screen->h;

		if (SDL_BlitSurface(i->Font_Screen, NULL, tmp, &rect))
		{
			SDL_FreeSurface(tmp);
			ClearFont_Screen(G_Text);
			return -1;
		}
		//SDL_SaveBMP( i->Font_Screen, "hello.bmp" );
	}

	//SDL_SaveBMP( tmp, "hello2.bmp" );

	glGenTextures(1, &hudTextureID);
	glBindTexture(GL_TEXTURE_2D, hudTextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nWidthPowerOfTwo, nHeightPowerOfTwo, 0, GL_RGBA, GL_UNSIGNED_BYTE, tmp->pixels);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glBegin(GL_QUADS);
	glTexCoord2i(0,1); glVertex3i(G_Text->x, G_Text->y+nHeightPowerOfTwo+1, 0);
	glTexCoord2i(0,0); glVertex3i(G_Text->x, G_Text->y+1, 0);
	glTexCoord2i(1,0); glVertex3i(G_Text->x+nWidthPowerOfTwo, G_Text->y+1, 0);
	glTexCoord2i(1,1); glVertex3i(G_Text->x+nWidthPowerOfTwo, G_Text->y+nHeightPowerOfTwo+1, 0);
	glEnd();

	glDeleteTextures(1,&hudTextureID);

	SDL_FreeSurface(tmp);
	ClearFont_Screen(G_Text);
#endif
	return 0;
}

}