/* $Id$ */
#ifndef __SDLENGINE_H
#define __SDLENGINE_H

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <SDL.h>

/**
 * Simple interface for initializing OpenGL support over SDL.
 * Just create a subclass from this one with a <b>virtual void Draw()</b>
 * function so that can be executed when calling Sync().
 */
class SDLEngine {
protected:
	unsigned int m_width, m_height;
	void ProcessKeyboard();

	bool keys[1024];

	SDLMod mod;
	SDLKey Lastsym,Lastsym2;
	long lastTick;
	long tickDelay;

public:
	SDLEngine();
	virtual ~SDLEngine();

	/**
	 * Initializes SDL and OpenGL
	 * @returns bool true on success
	 */
	bool InitDisplay(const unsigned int& = 640, const unsigned int& = 480, const bool& = false, const unsigned int& bpp = 16);
	void CloseDisplay();
	void Sync();
	virtual void WindowResize();

	bool getKey(unsigned int) const;

	/**
	 * Draws the image before Sync()
	 */
	virtual void Draw();
	virtual void BeforeClose();

	// Events
	virtual void evtQuit();
	virtual bool evtKeyPress(SDL_Event *sdlEvent, const int& mod = 0);
	virtual bool evtKeyRelease(SDL_Event *sdlEvent, const int& mod = 0);
	virtual bool evtMouseClick(const int& x, const int& y, const int& buttons);
	virtual bool evtMouseRelease(const int& x, const int& y, const int& buttons);
	virtual bool evtMouseMove(const int& x, const int& y, const int& dx, const int& dy);

	void setTransparency(bool) const;
};

#endif /* __SDLENGINE_H */