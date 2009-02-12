/* $Id$ */
#include "stdafx.h"

#include "ro_engine.h"
#include "sdl.h"

void ROEngine::HandleKeyboard() {}

void ROEngine::ReadIni(const std::string& name) {
	std::ifstream ini;
	std::string cmd, file;
	char buf[512];
	char* ptr;
	bool is_cmd;

	ini.open(name.c_str(), std::ifstream::in);
	if (!ini.good()) {
		std::cerr << "Can't read from INI file: " << name << std::endl;
		return;
	}

	while (!ini.eof()) {
		is_cmd = true;
		ini.getline(buf, 512);
		ptr = buf;
		cmd = "";
		file = "";
		while (*ptr == ' ')
			ptr++;

		if (ptr[0] == ';' || ptr[0] == '#' || ptr[0] == 0)
			continue;

		while (ptr[0] != ':' && ptr[0] != 0) {
			cmd += ptr[0];
			ptr++;
		}
		if (ptr[0] == 0) {
			std::cerr << "Error parsing line " << buf << std::endl;
			continue;
		}

		ptr++;
		while (*ptr == ' ')
			ptr++;

		while (ptr[0] != 0) {
			file += ptr[0];
			ptr++;
		}
		std::cout << "CMD: " << cmd << "\tFILE: " << file << std::endl;

		if (cmd == "grf") {
			if (!m_filemanager.OpenGRF(file))
				std::cerr << "Error opening GRF file " << file << std::endl;
		}
		else {
			std::cerr << "Error: Command '" << cmd << "' not supported." << std::endl;
		}
	}

	ini.close();
}


ROEngine::ROEngine(const std::string& name) : SDLEngine(name) {
	for (int i = 0; i < 1024; i++)
		keys[i] = false;

	m_quit = false;
}

ROEngine::~ROEngine() {
}

void ROEngine::BeforeRun() {
	InitDisplay();
}

void ROEngine::AfterRun() {
	m_gl_objects.clear();
	m_ro_objects.clear();
	m_texturemanager.Clear();
	CloseDisplay();
}

void ROEngine::evtQuit() {
	m_quit = true;
}

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

		cam.Look();
		m_frustum.Calculate();

		m_gl_objects.draw(m_frustum, tickDelay);

		AfterDraw();
		Sync();
	}
	AfterRun();
}


