/* $Id$ */
#ifndef __GUI_TEXTINPUT_H
#define __GUI_TEXTINPUT_H

#include "label.h"
#include "sdle/font.h"

typedef struct _Node {
        LCHAR Ch;
		bool  selected;
        
        struct _Node *Prev;
        struct _Node *Next;
		SDL_Surface *Font_Screen;
}NODE;

typedef struct _TextEditor {
	SDL_Surface	*Surface;           //目标表面
	sdle::Font	*Font;              //字体

	NODE		*Head;             //一行文字的开始
	NODE		*Start;            //一行文字的开始
	NODE		*Current;          //当前节点, 按左右等键移动此节点
	NODE		*select1;          //按住选择结点1
	NODE		*select2;          //按住选择结点2
        
	bool		Text_Changed;      //文件是否已改变, FALSE没有改变, TURE已改变       
        
	Sint16		x;
	Sint16		y;
	Sint16		z;
	Sint32		Len;
	int         EffectWidth;
	LCHAR		*m_text;
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
	static unsigned int cursor_delay;//当前光标闪烁等待时间
	/** Cursor Position */
	unsigned int m_pos;
	/** Cursor selection start */
	unsigned int m_start;
	unsigned int m_delay;
	bool m_bar,actived,m_MouseDown;

public:
	TextEditor G_Text;
	bool       SelectFlag;

	TextInput(Element* parent, const TiXmlElement*, TextureManager&, FileManager&);
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
	NODE* NewNode(OUT NODE *Front, OUT NODE *Behind, IN LCHAR Ch);
	void GetStringFromNode();

	bool RemoveLeft();
	bool RemoveRight();
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
