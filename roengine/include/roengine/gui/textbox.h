/* $Id: textbox.h $ */
#ifndef __GUI_TEXTBOX_H
#define __GUI_TEXTBOX_H

#include "element.h"

namespace GUI {

/** GUI object for displaying text */
class TextBox : public Element {
protected:
	int scrollPos;
	std::vector<std::string> m_text;
public:
	TextBox(Element* parent, CacheManager&);
	TextBox(Element* parent, const TiXmlElement*, CacheManager&);
	~TextBox();

	/** Draws the control
	 * @param delay ticks passed since last draw (to draw character position animation)
	 */
	virtual void Draw(unsigned int delay = 0);
	void Add(std::string s);
	void Clear();
/*	virtual void onGetFocus();
	virtual void onLoseFocus();
	virtual bool HandleKeyDown(SDL_Event *sdlEvent, int mod = 0);
	virtual bool HandleKeyUp(SDL_Event *sdlEvent, int mod = 0);
	virtual bool HandleMouseDown(int x, int y, int button);
	virtual bool HandleMouseRelease(int x, int y, int button);*/

/*	bool Insert(Uint16 Ch);
	NODE* NewNode(OUT NODE *Front, OUT NODE *Behind, IN LCHAR Ch);
	void GetStringFromNode();

	bool MoveLeft();
	bool MoveRight();
	bool MoveStart();
	bool MoveEnd();
	bool CheckPos();
	void FreeString();
	void ClickPos(int x);
	std::string& getText();*/
};

}

#endif /* __GUI_TEXTBOX_H */
