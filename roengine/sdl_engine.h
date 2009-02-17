/* $Id$ */
#ifndef __SDL_ENGINE_H
#define __SDL_ENGINE_H

#include <string>

class SDLEngine {
private:
	bool m_mode2d;
protected:
	unsigned int m_width, m_height;
	std::string caption;
	void ProcessKeyboard();

	bool keys[1024];
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
	virtual void evtKeyPress(const int& key);
	virtual void evtKeyRelease(const int& key);

	// Some primitives
	static void cube(const float& side = 1.0f);
};

#endif
