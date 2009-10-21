/* $Id$ */
#include "stdafx.h"
#include "sdle/sdl_engine.h"
#include "sdle/glf_font.h"

#include "sdle/arabia-8.glf.h"
#include "sdle/arial-8.glf.h"
#include "sdle/arial-10.glf.h"

#ifndef _MSC_VER
#	include <SDL/SDL.h>
#else
#	include <SDL.h>
#endif
#include <stdio.h>
#include <gl/gl.h>
#include <GL/glu.h>

namespace sdle {
bool SDLEngine::m_supportPot = false;

SDLEngine *SDLEngine::m_singleton = NULL;

SDLEngine::SDLEngine() {
	init();
}

SDLEngine::SDLEngine(const char* caption) {
	init();
	strcpy(this->caption, caption);
}

SDLEngine::~SDLEngine() {
    CloseDisplay();
	m_singleton = NULL;
}

SDLEngine* SDLEngine::getSingleton() {
	return(m_singleton);
}

void SDLEngine::init() {
	if (m_singleton != NULL)
		fprintf(stderr, "[ERROR] We are opening another instance of SDLEngine. This SHOULD NEVER HAPPEN!");

	m_singleton = this;

	m_width = m_height = 0;
    strcpy(this->caption, "SDL Engine");
    m_mode2d = false;

    near_clip = 20.0f;
    far_clip = 1000.0f;

	m_screenshot = false;
}

bool SDLEngine::InitDisplay(const unsigned int& w, const unsigned int& h, const bool& fullscreen, const unsigned int& bpp) {
    BeforeInit();
    unsigned int initflags = SDL_INIT_VIDEO;
#ifdef _DEBUG
    initflags |= SDL_INIT_NOPARACHUTE;
#endif
    if (SDL_Init(initflags) != 0) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return (false);
    }
    SDL_WM_SetCaption(caption, caption);

    // TODO: Check if those values are 0, so use the largest possible.
    m_width = w;
    m_height = h;

#ifdef _DEBUG
    const SDL_VideoInfo * vi = SDL_GetVideoInfo();
    printf("Video Info:\n");
    printf("\tHardware Surface: %d\n", vi->hw_available);
    printf("\tWindow Manager: %d\n", vi->wm_available);
    if (vi->hw_available)
        printf("\tVideo Memory: %u\n", vi->video_mem);
    else
        printf("\tNo Hardware acceleration!\n");

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
        return (false);
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
    data = (char*) glGetString(GL_VENDOR);
    printf("OpenGL Vendor: %s\n", data);
    data = (char*) glGetString(GL_VERSION);
    printf("       Version: %s\n", data);

    data = (char*) glGetString(GL_EXTENSIONS);
    const char* ptr = data;
    while (ptr[0] != 0) {
        if (ptr[0] == ' ')
			ptr++;
            if (!strncmp("GL_ARB_texture_non_power_of_two", ptr, strlen("GL_ARB_texture_non_power_of_two"))) {
                m_supportPot = true;
                break;
            }
        ptr++;
    }
#ifdef DEBUG
    if (m_supportPot) {
		printf("SDLEngine: We have POT!\n");
    }
	printf("SDLEngine:       Extensions: %s\n", data);
#endif

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LEQUAL);
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    WindowResize();
    glLoadIdentity();
    AfterInit();

	// Reset keys
	for (int i = 0; i < 1024; i++)
		keys[i] = false;

	GLFFont::Arabia8 = new GLFFont(glf_arabia8, glf_arabia8_size);
	GLFFont::Arial8 = new GLFFont(glf_arial8, glf_arial8_size);
	GLFFont::Arial10 = new GLFFont(glf_arial10, glf_arial10_size);

    return (true);
}

bool SDLEngine::supportPot() {
	return(m_supportPot);
}


void SDLEngine::CloseDisplay() {
	delete GLFFont::Arabia8;
	delete GLFFont::Arial8;
	delete GLFFont::Arial10;
    SDL_Quit();
}

const bool* SDLEngine::getKeys() const {
    return (keys);
}

void SDLEngine::setNearClip(float f) {
    near_clip = f;
    WindowResize();
}

void SDLEngine::setFarClip(float f) {
    far_clip = f;
    WindowResize();
}

void SDLEngine::WindowResize() {
    SDL_Surface* screen = SDL_GetVideoSurface();
    m_width = screen->w;
    m_height = screen->h;

    glViewport(0, 0, m_width, m_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ratio = (float) m_width / (float) m_height;
    gluPerspective(45.0f, ratio, near_clip, far_clip);
    //printf("gluPerspetive(%f,%f,%f,%f)\n",45.0f, ratio, near_clip, far_clip);
    glMatrixMode(GL_MODELVIEW);
}

void SDLEngine::LookAt(float sX, float sY, float sZ, float dX, float dY, float dZ) {
    gluLookAt(sX, sY, sZ, dX, dY, dZ, 0, 1, 0);
}

void SDLEngine::Sync(unsigned long delay) {
    if (m_mode2d)
        Mode2DEnd();
#ifdef SDLENGINE_CONSOLE
	m_console.Draw();
#endif
    SDL_GL_SwapBuffers();
	if (m_screenshot)
		SaveScreen();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	if (delay > 0)
		SDL_Delay(delay);
}

void SDLEngine::unProject(int x, int y, Vertex* ret) {
	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];
	int viewport[4];
	int realY;
	GLfloat winZ;

	double px, py, pz;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);

	realY = viewport[3] - y;

	glReadPixels(x, realY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	gluUnProject(x, realY, winZ, modelMatrix, projMatrix, viewport, &px, &py, &pz);

	ret->x = (float)px;
	ret->y = (float)py;
	ret->z = (float)pz;
}

void SDLEngine::Mode2DStart() {
	if (m_mode2d)
		return;
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, m_width, m_height, 0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);

    m_2dcache_blend_on = glIsEnabled(GL_BLEND);
    m_2dcache_texture_on = glIsEnabled(GL_TEXTURE_2D);

    if (!m_2dcache_texture_on) {
    	glEnable(GL_TEXTURE_2D);
    }

    if (m_2dcache_blend_on) {
    	glGetIntegerv(GL_BLEND_SRC, &m_2dcache_blend_src);
    	glGetIntegerv(GL_BLEND_DST, &m_2dcache_blend_dst);
    }
    else {
    	glEnable(GL_BLEND);
    }
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


    m_mode2d = true;
}

void SDLEngine::Mode2DEnd() {
	if (!m_mode2d)
		return;
    m_mode2d = false;

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

	if (m_2dcache_blend_on) {
		glBlendFunc(m_2dcache_blend_src, m_2dcache_blend_dst);
	}
	else {
		glDisable(GL_BLEND);
	}

    if (!m_2dcache_texture_on) {
    	glDisable(GL_TEXTURE_2D);
    }
}

bool SDLEngine::Mode2D() const {
	return(m_mode2d);
}

void SDLEngine::ProcessKeyboard() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_KEYDOWN:
                keys[e.key.keysym.sym] = true;
				m_mod = e.key.keysym.mod;
				m_lastkey = e.key.keysym.sym;
#ifdef SDLENGINE_CONSOLE
				if (!m_console.InjectKeyDown(&e))
#endif
                evtKeyPress(&e, e.key.keysym.mod);
                break;
            case SDL_KEYUP:
                keys[e.key.keysym.sym] = false;
				if (m_lastkey = e.key.keysym.sym)
					m_lastkey = SDLK_UNKNOWN;
#ifdef SDLENGINE_CONSOLE
				if (!m_console.InjectKeyUp(&e))
#endif
                evtKeyRelease(&e, e.key.keysym.mod);
                break;
            case SDL_MOUSEMOTION:
                evtMouseMove(e.button.x, e.button.y, e.motion.xrel, e.motion.yrel);
				//printf("SDLEngine: Mouse Motion (%d, %d) d(%d, %d)\n", e.button.x, e.button.y, e.motion.xrel, e.motion.yrel);
                break;
            case SDL_MOUSEBUTTONDOWN:
                evtMouseClick(e.button.x, e.button.y, e.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                evtMouseRelease(e.button.x, e.button.y, e.button.button);
                break;
            case SDL_QUIT:
                evtQuit();
                break;
        }
    }
}

void SDLEngine::evtQuit() {
}

bool SDLEngine::evtKeyPress(SDL_Event *sdlEvent, const int& mod) {
    return (false);
}

bool SDLEngine::evtKeyRelease(SDL_Event *sdlEvent, const int& mod) {
    return (false);
}

bool SDLEngine::evtMouseClick(const int& x, const int& y, const int& buttons) {
    return (false);
}

bool SDLEngine::evtMouseRelease(const int& x, const int& y, const int& buttons) {
    return (false);
}

bool SDLEngine::evtMouseMove(const int& x, const int& y, const int& dx, const int& dy) {
    return (false);
}

void SDLEngine::BeforeInit() {
}

void SDLEngine::AfterInit() {
}

unsigned int SDLEngine::getWidth() const {
	return(m_width);
}

unsigned int SDLEngine::getHeight() const {
	return(m_height);
}


int SDLEngine::Screenshot(char *filename) {
	strcpy(screenshot_fn, filename);
	m_screenshot = true;
	return(0);
}

void SDLEngine::SaveScreen() {
	m_screenshot = false;

    SDL_Surface *screen = SDL_GetVideoSurface();
    SDL_Surface *temp;
    unsigned char *pixels;
    int i;

    if (!(screen->flags & SDL_OPENGL)) {
        SDL_SaveBMP(screen, screenshot_fn);
        return;
    }

    temp = SDL_CreateRGBSurface(SDL_SWSURFACE, screen->w, screen->h, 24,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
            0x000000FF, 0x0000FF00, 0x00FF0000, 0
#else
            0x00FF0000, 0x0000FF00, 0x000000FF, 0
#endif
            );
    if (temp == NULL)
        return;

    pixels = (unsigned char*)malloc(3 * screen->w * screen->h);
    if (pixels == NULL) {
        SDL_FreeSurface(temp);
        return;
    }

    glReadPixels(0, 0, screen->w, screen->h, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    for (i = 0; i < screen->h; i++)
        memcpy(((char *) temp->pixels) + temp->pitch * i, pixels + 3 * screen->w * (screen->h - i - 1), screen->w * 3);
    free(pixels);

    SDL_SaveBMP(temp, screenshot_fn);
    SDL_FreeSurface(temp);
    return;
}

/* THE PRIMITIVES */

void SDLEngine::cube(const float& side) {
    float aux = side / 2;
    // Thanks, NeHe! http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=06
    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-aux, -aux, aux); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(aux, -aux, aux); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(aux, aux, aux); // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-aux, aux, aux); // Top Left Of The Texture and Quad
    // Back Face
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-aux, -aux, -aux); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-aux, aux, -aux); // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(aux, aux, -aux); // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(aux, -aux, -aux); // Bottom Left Of The Texture and Quad
    // Top Face
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-aux, aux, -aux); // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-aux, aux, aux); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(aux, aux, aux); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(aux, aux, -aux); // Top Right Of The Texture and Quad
    // Bottom Face
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-aux, -aux, -aux); // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(aux, -aux, -aux); // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(aux, -aux, aux); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-aux, -aux, aux); // Bottom Right Of The Texture and Quad
    // Right face
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(aux, -aux, -aux); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(aux, aux, -aux); // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(aux, aux, aux); // Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(aux, -aux, aux); // Bottom Left Of The Texture and Quad
    // Left Face
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-aux, -aux, -aux); // Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-aux, -aux, aux); // Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-aux, aux, aux); // Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-aux, aux, -aux); // Top Left Of The Texture and Quad
    glEnd();
}

// HELPERS

void SDLEngine::enable(unsigned int v) {
    glEnable(v);
}

void SDLEngine::disable(unsigned int v) {
    glDisable(v);
}

void SDLEngine::enableTexture2D() {
    enable(GL_TEXTURE_2D);
}

void SDLEngine::disableTexture2D() {
    disable(GL_TEXTURE_2D);
}

#ifdef SDLENGINE_CONSOLE
Console& SDLEngine::getConsole() {
	return(m_console);
}
#endif
}
