/* $Id$ */
#include "stdafx.h"
#include "sdlengine.h"

SDLEngine::SDLEngine() {
	m_width = m_height = 0;
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
	SDL_WM_SetCaption("Open Ragnarok Project", "ORP");

	// TODO: Check if those values are 0, so use the largest possible.
	m_width = w;
	m_height = h;

	const SDL_VideoInfo * vi = SDL_GetVideoInfo();
	printf("Video Info:\n");
	printf("\tHardware Surface: %d\n", vi->hw_available);
	printf("\tWindow Manager: %d\n", vi->wm_available);
	if (vi->hw_available) 
		printf("\tVideo Memory: %u\n", vi->video_mem);
	
	int flags = SDL_OPENGL | SDL_ANYFORMAT;

	if (fullscreen)
		flags |= SDL_FULLSCREEN;

#define HWCHECK
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

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.1f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	WindowResize();
	glLoadIdentity();
	return(true);
}

void SDLEngine::CloseDisplay() {
	BeforeClose();
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
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

void SDLEngine::Sync() {
	Draw();
	SDL_GL_SwapBuffers();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
}

void SDLEngine::setTransparency(bool t) const {
	if (t) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else {
		glDisable(GL_BLEND);
	}
}

void SDLEngine::Draw() {
}

void SDLEngine::BeforeClose() {
}