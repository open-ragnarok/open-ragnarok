/* $Id$ */
#ifndef __SDL_ENGINE_H
#define __SDL_ENGINE_H

#include <string>

class SDLEngine {
protected:
	unsigned int m_width, m_height;
	std::string caption;
public:
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

	// Some primitives
	static void cube(const float& side = 1.0f);
};

#endif
