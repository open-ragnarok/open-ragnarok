/* $Id$ */
#include "stdafx.h"

#include "roengine/ro_engine.h"
#include "sdle/sdl_engine.h"

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

int file_exists (char* fileName) {
	struct stat buf;
	int i = stat ( fileName, &buf );
	if (i == 0) {
		return 1;
	}
	return 0;
}

void ROEngine::HandleKeyboard() {}

void ROEngine::clearActors() {
	std::map<unsigned int, Actor*>::iterator itr;

	itr = m_actors.begin();
	while (itr != m_actors.end()) {
		delete(itr->second);
		itr++;
	}

	m_actors.clear();
}

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

	// Now that we have our source files loaded, we can start reading the tables...
	
	// resnametable.txt
	if (m_filemanager.fileExists("resnametable.txt")) {
		FileData data = m_filemanager.getFile("resnametable.txt");
		char line[512];
		char a[256], b[256];
		unsigned int linepos;
		unsigned int pos = 0;
		char c;
		while(pos < data.blobSize()) {
			memset(line, 0, 512);
			memset(a, 0, 256);
			memset(b, 0, 256);
			linepos = 0;
			// Read the line
			c = data[pos++];
			while (pos < data.blobSize()) {
				if (c == 13 || c == 0 || c == 10) {
					if (linepos == 0) {
						c = data[pos++];
						continue;
					}
					break;
				}
				line[linepos++] = c;
				c = data[pos++];
			}
			// Process the line
			linepos = 0;
			while (line[linepos] != '#' && line[linepos] != 0)
				linepos++;
			if (linepos >= strlen(line)) {
				// Comments and stuff
				continue;
			}
			strncpy(a, line, linepos);
			strcpy(b, line + linepos + 1);
			b[strlen(b) - 1] = 0;
			//printf("%s => %s\n", a, b);
			if (strcmp(a,b) != 0) {
				m_filemanager.addName(a, b);
			}
		}
	}

	ReadTable("npclist.txt", m_npc_names);
	ReadTable("joblist.txt", m_job_names);
	ReadTable("homunculuslist.txt", m_homunculus_names);
	ReadTable("mercenarylist.txt", m_mercenary_names);
	ReadTable("moblist.txt", m_mob_names);
}

bool ROEngine::ReadTable(const char* fn, std::map<unsigned short, std::string>& list) {
	if (!m_filemanager.fileExists(fn)) {
		return(false);
	}

	list.clear();
	FileData data = m_filemanager.getFile(fn);
	char line[512];
	char a[256], b[256];
	unsigned int linepos;
	unsigned int pos = 0;
	char c;
	while(pos < data.blobSize()) {
		memset(line, 0, 512);
		memset(a, 0, 256);
		memset(b, 0, 256);
		linepos = 0;
		// Read the line
		c = data[pos++];
		while (pos < data.blobSize()) {
			if (c == 13 || c == 0 || c == 10) {
				if (linepos == 0) {
					c = data[pos++];
					continue;
				}
				break;
			}
			line[linepos++] = c;
			c = data[pos++];
		}
		// Process the line
		linepos = 0;
		while (line[linepos] != '#' && line[linepos] != 0)
			linepos++;
		if (linepos >= strlen(line)) {
			// Comments and stuff
			continue;
		}
		strncpy(a, line, linepos);
		strcpy(b, line + linepos + 1);
		b[strlen(b) - 1] = 0;
		list[atoi(a)] = b;
	}

	return(true);
}


ROEngine::ROEngine(const std::string& name) : SDLEngine(name.c_str()) {
	for (int i = 0; i < 1024; i++)
		keys[i] = false;

	m_quit = false;
	m_cursorSprite = 0;
	m_map = NULL;
	cam.getEye().set(200, 200, -200);
	m_rotating = false;
	m_overactor = NULL;
	m_drawmap = true;
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
}

void ROEngine::evtQuit() {
	m_quit = true;
}

void ROEngine::BeforeDraw() {}
void ROEngine::AfterDraw() {}

void ROEngine::AfterInit() {
	m_gui.Init(getWidth(), getHeight());
}

void ROEngine::beforeDrawMap() {}

void ROEngine::afterDrawMap() {}

void ROEngine::DrawMap() {
	if (!m_drawmap || (m_map == NULL))
		return;

	beforeDrawMap();

	Vector3f camera_look;

	float wx, wy, wz;
	int mapx, mapy;
	cam.Look();
	m_cameradir = cam.getDirection();
	m_frustum.Calculate();
	m_map->setMouse(mousex, mousey);
	m_map->Render(tickDelay, &m_frustum);
	wx = m_map->getWorldX();
	wy = m_map->getWorldY();
	wz = m_map->getWorldZ();

	mapx = m_map->getMouseMapX();
	mapy = m_map->getMouseMapY();
	m_texturemanager["openro\\selected.png"].Activate();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	m_map->DrawSelection(mapx, mapy);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	//printf("World: %.2f, %.2f, %.2f", wx, wy, wz);
	//printf("\tMap: %d, %d", mapx, mapy);

	glPushMatrix();
	m_map->getWorldPosition((float)mapx, (float)mapy, &wx, &wy, &wz);
	glPopMatrix();

	//printf("\tW: %.2f, %.2f, %.2f", wx, wy, wz);
	//printf("\r");
	camera_look = cam.getEye() - cam.getDest();
	//m_gl_objects.draw(m_frustum, tickDelay, camera_look);


	// Positions the camera so we're right above ourselves.
	glPushMatrix();
	m_map->getWorldPosition(me.getPositionX(), me.getPositionY(), &wx, &wy, &wz);
	glPopMatrix();
	cam.TranslateDestTo(Vector3f(wx, wy, wz));

	if (me.valid()) {
		me.setMap(m_map);
		me.Render(50, &m_frustum, m_cameradir);
	}

	std::map<unsigned int, Actor*>::iterator itr;
	itr = m_actors.begin();
	while (itr != m_actors.end()) {
		if (itr->second->isVisible())
			itr->second->Render(50, &m_frustum, m_cameradir);
		itr++;
	}

	afterDrawMap();
}


void ROEngine::Run() {
	long curtick;
	m_quit = false;
	curtick = SDL_GetTicks();

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
		DrawMap();

		m_gui.Draw(tickDelay);

		//TODO: This is redundant. 2D mode should start in GUI and finish here. Maybe put this inside the Gui::Draw?
		Mode2DStart();
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.45f);
		ProcessMouse();
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_2D);
		Mode2DEnd();
		
		m_gui.ProcessEvents();

		AfterDraw();
		Sync();
	}
	getCursor().Clear();
	AfterRun();
}

bool ROEngine::evtKeyPress(SDL_Event *sdlEvent, const int& mod) {
	if (sdlEvent->key.keysym.sym == SDLK_PRINT) {
		unsigned int i = 1;
		char fn[128];
		sprintf(fn, "screenshot-%d.bmp", i);
		while (file_exists(fn)) {
			i++;
			sprintf(fn, "screenshot-%d.bmp", i);
		}
		this->Screenshot(fn);
		printf("Screenshot saved to file '%s'.", fn);
		return(true);
	}
	return(m_gui.InjectKeyPress(sdlEvent, mod));
}

bool ROEngine::evtKeyRelease(SDL_Event *sdlEvent, const int& mod) {
	return(m_gui.InjectKeyRelease(sdlEvent, mod));
}

bool ROEngine::evtMouseClick(const int& x, const int& y, const int& buttons) {
	bool ret = m_gui.InjectMouseClick(x, y, buttons);
	if (ret == false) {
		if (buttons == 3) { // SDL_BUTTON_RIGHT
			m_rotating = true;
			return(true);
		}
		else if (buttons == 1) { // SDL_BUTTON_LEFT
			if (m_map != NULL) {
				int mapx, mapy;
				mapx = m_map->getMouseMapX();
				mapy = m_map->getMouseMapY();

				if (m_overactor != NULL) {
					if (m_overactor->typeID == 45) {
						// Yeah, yeah... We click on the portal and move to it.
						// The server handles the warp by walking into the portal location
						// Kudos to FlavioJS.
						clickPortal(mapx, mapy, (NpcObj*)m_overactor);
						clickMap(mapx, mapy);
					}
					else {
						switch(m_overactor->getType()) {
							case Actor::NpcType:
								clickNpc(mapx, mapy, (NpcObj*)m_overactor);
								break;
							case Actor::MobType:
								clickMob(mapx, mapy, (MobObj*)m_overactor);
								break;
							default:
								printf("Unhandled actor click. Type %d\n", m_overactor->typeID);
						}
					}
				}
				else {
					clickMap(mapx, mapy);
				}
			}
		}
		else if (buttons == 5) { // SDL_BUTTON_WHEELDOWN
			cam.ZoomOut(8.0f);
		}
		else if (buttons == 4) { // SDL_BUTTON_WHEELUP
			cam.ZoomIn(8.0f);
		}
	}

	return(ret);
}

bool ROEngine::evtMouseRelease(const int& x, const int& y, const int& buttons) {
	if (buttons & SDL_BUTTON_RIGHT) {
		if (m_rotating) {
			m_rotating = false;
			return(true);
		}
	}
	return(m_gui.InjectMouseRelease(x, y, buttons));
}

bool ROEngine::evtMouseMove(const int& x, const int& y, const int& dx, const int& dy) {
	//Save the mouse pos to draw the spr cursor
	mousex = x;
	mousey = y;
	//printf("ROEngine:: Moving mouse (%d, %d), d(%d, %d)\n", x, y, dx, dy);

	m_overactor = NULL;

	if (m_map != NULL) {

		if (m_rotating) {
			cam.Rotate((float)dx / 10);
			return(true);
		}

		int mapx, mapy;
		mapx = m_map->getMouseMapX();
		mapy = m_map->getMouseMapY();

		std::map<unsigned int, Actor*>::iterator itr = m_actors.begin();
		while (itr != m_actors.end()) {
			Actor* actor = itr->second;
			if (actor->getPositionX() == mapx && actor->getPositionY() == mapy) {
				m_overactor = actor;
				break;
			}						
			itr++;
		}
	}

	return(m_gui.InjectMouseMove(x, y, dx, dy));
}

void ROEngine::ProcessMouse(int xless, int yless){
	//Change the cursor sprite every 100ms

	int action = 0;
	int maxspr;
	if (m_rotating) {
		action = 4;
	}
	else if (m_overactor != NULL) {
		// TODO: Check if Another player
		if (m_npc_names.find(m_overactor->typeID) != m_npc_names.end()) {
			action = 1; // npc
			if (m_overactor->typeID == 45)
				action = 7; // portal
		}
		else {
			action = 5; // attack
		}
	}

	maxspr = getCursor().getAct()->getMotionCount(action);

	m_cursorTick += tickDelay;
	m_cursorTick = m_cursorTick % (100 * maxspr);
	while (m_cursorTick >= 100) {
		m_cursorSprite++;
		m_cursorTick -= 100;
	}

	m_cursorSprite = m_cursorSprite % maxspr;


	//TODO: Put a check to know if the cursor is over a button to change the sprite to a "hand".. etc
	DrawFullAct(getCursor(), (float)(getMouseX() - xless), (float)(getMouseY() - yless), action, m_cursorSprite, false, NULL, true, false);
}

/** Sets the current map */
void ROEngine::setMap(RswObject* map) {
	m_map = map;
}

Frustum& ROEngine::getFrustum() { return(m_frustum); }
FullAct& ROEngine::getCursor(){return cursor;}
void ROEngine::setCursor(FullAct& cur){cursor = cur;}
int ROEngine::getMouseX(){return mousex;}
int ROEngine::getMouseY(){return mousey;}

// Placeholders
void ROEngine::clickMap(int x, int y) {}
void ROEngine::clickMob(int x, int y, MobObj* obj) {
	printf("Clicked Mob 0x%08x at %d,%d\n", obj->id, x, y);
}
void ROEngine::clickItem(int x, int y) {}
void ROEngine::clickPortal(int x, int y, NpcObj* npc) {
	printf("Clicked Portal 0x%08x at %d,%d\n", npc->id, x, y);
}
void ROEngine::clickNpc(int x, int y, NpcObj* npc) {
	printf("Clicked NPC 0x%08x at %d,%d\n", npc->id, x, y);
}

