/* $Id$ */
#include "stdafx.h"

#include "ro_engine.h"
#include "sdl.h"

void ROEngine::ProcessKeyboard() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_KEYDOWN:
				keys[event.key.keysym.sym] = true;
				break;
			case SDL_KEYUP:
				keys[event.key.keysym.sym] = false;
				break;
			case SDL_QUIT:
				m_quit = true;
				break;
		}
	}
}

void ROEngine::HandleKeyboard() {}

ROEngine::ROEngine(const std::string& name) : SDLEngine(name) {
	for (int i = 0; i < 1024; i++)
		keys[i] = false;

	m_quit = false;
}

ROEngine::~ROEngine() {
}

void ROEngine::BeforeRun() {}
void ROEngine::AfterRun() {}
void ROEngine::BeforeDraw() {}
void ROEngine::AfterDraw() {}

void ROEngine::Run() {
	long curtick;
	m_quit = false;
	BeforeRun();
	while (!m_quit) {
		ProcessKeyboard();

		curtick = SDL_GetTicks();
		tickDelay = curtick - lastTick;
		lastTick = curtick;

		//printf("delay: %d\tctick: %d\t\t\r", tickDelay, curtick);

		HandleKeyboard();

		BeforeDraw();
		m_gl_objects.draw(tickDelay);
		AfterDraw();
		Sync();
	}
	AfterRun();
}


