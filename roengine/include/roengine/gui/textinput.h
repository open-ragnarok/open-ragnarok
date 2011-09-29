/* $Id$ */
#ifndef __GUI_TEXTINPUT_H
#define __GUI_TEXTINPUT_H

#include "label.h"
#include "sdle/font.h"

typedef struct _Node {
        Uint16 UnicodeCh;
		bool  selected;
        
        struct _Node *Prev;
        struct _Node *Next;
		SDL_Surface *Font_Screen;
}NODE;

typedef struct _TextEditor {
	SDL_Surface	*Surface;           //目眮E丒
	//sdle::Font	*Font;              //字虂E

	NODE		*Head;             //一行文字的开始
	NODE		*Start;            //一行文字的开始
	NODE		*Current;          //当前节祦E 按左右等紒E贫私诘丒
	NODE		*select1;          //按住选择结祦E
	NODE		*select2;          //按住选择结祦E
        
	bool		Text_Changed;      //文件是否已改眮E FALSE没有改眮E TURE已改眮E      
        
	Sint16		x;
	Sint16		y;
	Sint16		z;
	Sint32		Len;
	int         EffectWidth;
	Uint16		*m_text;
	Uint32		FColor;
	Uint32		BColor;
	Uint32		SFColor;
	Uint32		SBColor;
	Uint8		Alpha;
}TextEditor;

namespace GUI {

typedef struct {
	int startPos;
	int endPos;
}LINEINFO;

/** GUI object for inputing text */
class TextInput : public Label {
protected:
	static unsigned int cursor_delay;//当前光眮E了傅却奔丒
	/** Cursor Position */
	unsigned int m_pos;
	/** Cursor selection start */
	int selStart, selEnd;
	int len;
	unsigned int m_start;
	unsigned int m_delay;
	bool m_bar,actived,m_MouseDown;
	SDLKey m_key;
	unsigned short repeatingDelay;

public:
	TextEditor G_Text;
	bool       SelectFlag;

	TextInput(Element* parent, const TiXmlElement*, CacheManager&);
	~TextInput();

	/** Draws the control
	 * @param delay ticks passed since last draw (to draw character position animation)
	 */
	virtual void Draw(unsigned int delay = 0);
	virtual void onGetFocus();
	virtual void onLoseFocus();
	virtual bool HandleKeyDown(SDL_Event *sdlEvent, int mod = 0);
	virtual bool HandleKeyUp(SDL_Event *sdlEvent, int mod = 0);
	virtual bool HandleMouseDown(int x, int y, int button);
	virtual bool HandleMouseRelease(int x, int y, int button);

	bool Insert(Uint16 Ch);
	NODE* NewNode(NODE *Front, NODE *Behind, Uint16 UnicodeCh);
	void GetStringFromNode();

	bool RemoveLeft();
	bool RemoveRight();
	bool RemoveSelected();
	bool MoveLeft();
	bool MoveRight();
	bool MoveStart();
	bool MoveEnd();
	bool CheckPos();
	void FreeString();
	void SelectAll();
	void UnSelectAll();
	int GetCursorX();
	void ClickPos(int x);
	std::string& getText();
};

}

#endif /* __GUI_TEXTINPUT_H */
