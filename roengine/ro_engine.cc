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
			if (ptr[0] == ';' || ptr[0] == '#')
				break;
			file += ptr[0];
			ptr++;
		}
		std::cout << "CMD: " << cmd << "\tFILE: " << file << std::endl;

		if (cmd == "grf") {
			if (!m_filemanager.OpenGRF(file))
				std::cerr << "Error opening GRF file " << file << std::endl;
		}
		else if (cmd == "fs") {
			if (!m_filemanager.OpenFS(file))
				std::cerr << "Error opening location " << file << std::endl;
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
	m_gui.clear();
	m_texturemanager.Clear();
	CloseDisplay();
}

void ROEngine::evtQuit() {
	m_quit = true;
}

void ROEngine::BeforeDraw() {}
void ROEngine::AfterDraw() {}

void ROEngine::AfterInit() {
	m_gui.Init(m_width, m_height);
}

void ROEngine::Run() {
	long curtick;
	m_quit = false;

	Vector3f camera_look;

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

		camera_look = cam.getEye() - cam.getDest();
		m_gl_objects.draw(m_frustum, tickDelay, camera_look);
		m_gui.Draw(tickDelay);
		m_gui.ProcessEvents();

		AfterDraw();
		Sync();
	}
	AfterRun();
}

bool ROEngine::evtKeyPress(const int& key, const int& mod) {
	return(m_gui.InjectKeyPress(key, mod));
}

bool ROEngine::evtKeyRelease(const int& key, const int& mod) {
	return(m_gui.InjectKeyRelease(key, mod));
}

bool ROEngine::evtMouseClick(const int& x, const int& y, const int& buttons) {
	return(m_gui.InjectMouseClick(x, y, buttons));
}

TextureManager& ROEngine::getTextureManager() { return(m_texturemanager); }
GLObjectCache& ROEngine::getGLObjects() { return(m_gl_objects); }
ROObjectCache& ROEngine::getROObjects() { return(m_ro_objects); }
FileManager& ROEngine::getFileManager() { return(m_filemanager); }
Frustum& ROEngine::getFrustum() { return(m_frustum); }