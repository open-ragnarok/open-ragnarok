/* $Id$ */
#include "stdafx.h"
#include "sdl_engine.h"

#include "sdl.h"

#include <stdio.h>

bool SDLEngine::supportPot = false;

SDLEngine::SDLEngine() {
	m_width = m_height = 0;
	caption = "Open-Ragnarok.org";
	m_mode2d = false;
}

SDLEngine::SDLEngine(const std::string& caption) {
	m_width = m_height = 0;
	this->caption = caption;
	m_mode2d = false;
}

SDLEngine::~SDLEngine() {
	CloseDisplay();
}

bool SDLEngine::InitDisplay(const unsigned int& w, const unsigned int& h, const bool& fullscreen, const unsigned int& bpp) {
	unsigned int initflags = SDL_INIT_VIDEO;
#ifdef _DEBUG
	initflags |= SDL_INIT_NOPARACHUTE;
#endif
	if (SDL_Init(initflags) != 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return(false);
	}
	SDL_WM_SetCaption(caption.c_str(), caption.c_str());

	// TODO: Check if those values are 0, so use the largest possible.
	m_width = w;
	m_height = h;

	const SDL_VideoInfo * vi = SDL_GetVideoInfo();
#ifdef _DEBUG
	printf("Video Info:\n");
	printf("\tHardware Surface: %d\n", vi->hw_available);
	printf("\tWindow Manager: %d\n", vi->wm_available);
	if (vi->hw_available) 
		printf("\tVideo Memory: %u\n", vi->video_mem);

#ifdef GL_TEXTURE_RECTANGLE_EXT
	printf("\tGL_TEXTURE_RECTANGLE_EXT defined.\n");
#else
	printf("\tGL_TEXTURE_RECTANGLE_EXT undefined.\n");
#endif /* GL_TEXTURE_RECTANGLE_EXT */

#endif /* _DEBUG */
	int flags = SDL_OPENGL | SDL_ANYFORMAT;

	if (fullscreen)
		flags |= SDL_FULLSCREEN;

//#define HWCHECK
#ifdef HWCHECK
	if (vi->hw_available)
		flags |= SDL_HWSURFACE;
	else
		flags |= SDL_SWSURFACE;
#else
	flags |= SDL_HWSURFACE;
#endif

	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, bpp);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_putenv("SDL_VIDEO_CENTERED=center");

	if (!SDL_SetVideoMode(m_width, m_height, bpp, flags)) {
		// TODO: Write log
		fprintf(stderr, "ERROR in SDL_SetVideoMode: %s\n", SDL_GetError());
		return(false);
	}

#ifdef WIN32
	/* http://www.libsdl.org/cgi/docwiki.cgi/SDL_5fSetVideoMode
	 * in Windows, setting the video mode "resets" OpenGL context. You must execute again
	 * the OpenGL initialization code (set the clear color or the shade model, for example)
	 * after calling SDL_SetVideoMode. In Linux, however, it works fine, and the
	 * initialization code only needs to be executed after the first call to
	 * SDL_SetVideoMode (although there is no harm in executing the initialization code
	 * after each call to SDL_SetVideoMode, for example for a multiplatform application).
	 */
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, bpp);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
#endif // WIN32

	const char* data;
	data = (char*)glGetString(GL_VENDOR);
	printf("OpenGL Vendor: %s\n", data);
	data = (char*)glGetString(GL_VERSION);
	printf("       Version: %s\n", data);

	data = (char*)glGetString(GL_EXTENSIONS);
	const char* ptr = data;
	while (ptr[0] != 0) {
		if (ptr[0] == ' ')
			if (!strncmp("GL_ARB_texture_non_power_of_two", ptr, strlen("GL_ARB_texture_non_power_of_two"))) {
				supportPot = true;
				break;
			}
		ptr++;
	}
	printf("       Extensions: %s\n", data);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.1f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	WindowResize();
	glLoadIdentity();
	return(true);
}

void SDLEngine::CloseDisplay() {
	SDL_Quit();
}

void SDLEngine::WindowResize() {
	SDL_Surface* screen = SDL_GetVideoSurface();
	m_width = screen->w;
	m_height = screen->h;
	
	glViewport(0, 0, m_width, m_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float ratio = (float)m_width / (float)m_height;
	gluPerspective(45.0f, ratio, 20.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
}

void SDLEngine::LookAt(float sX, float sY, float sZ, float dX, float dY, float dZ) {
	gluLookAt(sX, sY, sZ, dX, dY, dZ, 0, 1, 0);
}

void SDLEngine::Sync() {
	if (m_mode2d)
		Mode2DEnd();
	SDL_GL_SwapBuffers();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
}

void SDLEngine::Mode2DStart() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, m_width, m_height, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	m_mode2d = true;
}

void SDLEngine::Mode2DEnd() {
	m_mode2d = false;

	glEnable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void SDLEngine::ProcessKeyboard() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_KEYDOWN:
				keys[event.key.keysym.sym] = true;
				evtKeyPress(event.key.keysym.sym);
				break;
			case SDL_KEYUP:
				keys[event.key.keysym.sym] = false;
				evtKeyRelease(event.key.keysym.sym);
				break;
			case SDL_QUIT:
				evtQuit();
				break;
		}
	}
}

void SDLEngine::evtQuit() {}
void SDLEngine::evtKeyPress(const int& key) {}
void SDLEngine::evtKeyRelease(const int& key) {}



/* THE PRIMITIVES */

void SDLEngine::cube(const float& side) {
	float aux = side / 2;
	// Thanks, NeHe! http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=06
	glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-aux, -aux,  aux);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f( aux, -aux,  aux);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( aux,  aux,  aux);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-aux,  aux,  aux);	// Top Left Of The Texture and Quad
		// Back Face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-aux, -aux, -aux);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-aux,  aux, -aux);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( aux,  aux, -aux);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( aux, -aux, -aux);	// Bottom Left Of The Texture and Quad
		// Top Face
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-aux,  aux, -aux);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-aux,  aux,  aux);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f( aux,  aux,  aux);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( aux,  aux, -aux);	// Top Right Of The Texture and Quad
		// Bottom Face
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-aux, -aux, -aux);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( aux, -aux, -aux);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( aux, -aux,  aux);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-aux, -aux,  aux);	// Bottom Right Of The Texture and Quad
		// Right face
		glTexCoord2f(1.0f, 0.0f); glVertex3f( aux, -aux, -aux);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( aux,  aux, -aux);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( aux,  aux,  aux);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( aux, -aux,  aux);	// Bottom Left Of The Texture and Quad
		// Left Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-aux, -aux, -aux);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-aux, -aux,  aux);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-aux,  aux,  aux);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-aux,  aux, -aux);	// Top Left Of The Texture and Quad
	glEnd();
}