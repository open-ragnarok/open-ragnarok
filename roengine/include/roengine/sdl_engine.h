/* $Id$ */
#ifndef __SDL_ENGINE_H
#define __SDL_ENGINE_H

#include <string>
#ifdef _MSC_VER
#	include <SDL.h>
#else
#	include <SDL/SDL.h>
#endif

class SDLEngine {
private:
	bool m_mode2d;
protected:
	unsigned int m_width, m_height;
	std::string caption;
	void ProcessKeyboard();

	bool keys[1024];
	SDLMod mod;
	SDLKey Lastsym,Lastsym2;
	long lastTick;
	long tickDelay;
	virtual void BeforeInit();
	virtual void AfterInit();

public:

	static bool supportPot;

	SDLEngine();
	SDLEngine(const std::string& caption);
	virtual ~SDLEngine();

	/**
	 * Initializes SDL and OpenGL
	 * @returns bool true on success
	 */
	bool InitDisplay(const unsigned int& = 640, const unsigned int& = 480, const bool& = false, const unsigned int& bpp = 16);
	void CloseDisplay();
	void Sync();
	virtual void WindowResize();
	void LookAt(float sX, float sY, float sZ, float dX, float dY, float dZ);

	void Mode2DStart();
	void Mode2DEnd();

	// Events
	virtual void evtQuit();
	virtual bool evtKeyPress(const int& key, const int& mod = 0);
	virtual bool evtKeyRelease(const int& key, const int& mod = 0);
	virtual bool evtMouseClick(const int& x, const int& y, const int& buttons);
	virtual bool evtMouseRelease(const int& x, const int& y, const int& buttons);
	virtual bool evtMouseMove(const int& x, const int& y, const int& dx, const int& dy);

	// Some primitives
	static void cube(const float& side = 1.0f);
};

#endif
