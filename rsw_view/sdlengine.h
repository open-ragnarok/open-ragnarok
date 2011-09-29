/* $Id$ */
#ifndef __SDLENGINE_H
#define __SDLENGINE_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>

/**
 * Simple interface for initializing OpenGL support over SDL.
 * Just create a subclass from this one with a <b>virtual void Draw()</b>
 * function so that can be executed when calling Sync().
 *
 * <b>Note on stereo rendering:</b> Stereo rendering will only be useful if 
 * using the internal Draw function. If any drawing is being done before 
 * calling the Sync() function, it won't work.
 */
class SDLEngine {
protected:
	unsigned int m_width, m_height;
public:
	/** True if drawing stereo images */
	bool m_stereo;
	/** True if drawing stereo anaglyph images (RED/BLUE) */
	bool m_anaglyph;

	float z_near, z_far;

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

	/**
	 * Draws the image before Sync()
	 */
	virtual void Draw();
	virtual void BeforeClose();

	void setTransparency(bool) const;
};

#endif /* __SDLENGINE_H */
