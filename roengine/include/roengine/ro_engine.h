/* $Id$ */
#ifndef __RO_ENGINE_H
#define __RO_ENGINE_H

#include "sdle/sdl_engine.h"
#include "ro_object_cache.h"
#include "gl_object_cache.h"
#include "texturemanager.h"
#include "file_manager.h"
#include "frustum.h"
#include "camera.h"

#include "cache_manager.h"

#include "roengine/objects/cube_object.h"
#include "roengine/objects/rsm_object.h"
#include "roengine/objects/rsw_object.h"
#include "roengine/objects/charobj.h"
#include "roengine/objects/npcobj.h"

#include "ro/ro.h"
#include "rogl/rogl.h"
#ifdef _MSC_VER
#	include <SDL.h>
#else
#	include <SDL/SDL.h>
#endif
#include "gui/gui.h"
#include "roengine/full_act.h"

class ROEngine : public sdle::SDLEngine, public CacheManager {
protected:
	/**
	 * Object to calculate what's visible to optimize drawing time
	 */
	Frustum m_frustum;
	
	/**
	 * The graphic user interface
	 */
	GUI::Gui m_gui;

	/** Indicates if we're in a process of rotation or not */
	bool m_rotating;

	/**
	 * The cursor
	 */
	FullAct cursor;

	/**
	 * The Object representing us.
	 */
	CharObj me;

	/** Indicates the current cursor sprite in use */
	int m_cursorSprite;

	/** Stores the tick count to handle mouse animation */
	long m_cursorTick;

	bool m_quit;

	virtual void HandleKeyboard();
	virtual void BeforeRun();
	virtual void AfterRun();
	virtual void BeforeDraw();
	virtual void AfterDraw();
	virtual void AfterInit();

	long lastTick;

	/** How many ticks have passed since last frame */
	long tickDelay;

	Camera cam;

	/** The Direction that the camera is pointing */
	RO::CDir m_cameradir;

	int mousex;
	int mousey;

	/**
	 * The active map.
	 * Will be drawn if not null
	 */
	RswObject* m_map;

	/**
	 * All the actors that the user can currently see
	 */
	std::map<unsigned int, Actor*> m_actors;

	/**
	 * Clears all the actors (useful when changing maps)
	 */
	void clearActors();
	
	std::map<unsigned short, std::string> m_npc_names;

public:
	ROEngine(const std::string& name = "");
	~ROEngine();

	void Run();

	void ReadIni(const std::string& name = "data.ini");

	/** Event raised when the user left-clicks a map and there is nothing (mob, items, portals, etc) on the clicked place */
	virtual void clickMap(int x, int y);
	virtual void clickMob(int x, int y);
	virtual void clickItem(int x, int y);
	virtual void clickPortal(int x, int y);

	virtual void evtQuit();

	virtual bool evtKeyPress(SDL_Event *sdlEvent, const int& mod = 0);
	virtual bool evtKeyRelease(SDL_Event *sdlEvent, const int& mod = 0);
	virtual bool evtMouseClick(const int& x, const int& y, const int& buttons);
	virtual bool evtMouseRelease(const int& x, const int& y, const int& buttons);
	virtual bool evtMouseMove(const int& x, const int& y, const int& dx, const int& dy);

	Frustum& getFrustum();
	FullAct& getCursor();
	void setCursor(FullAct& cur);

	/** Sets the current map */
	void setMap(RswObject*);
	
	/** Process mouse animation and drawing */
	void ProcessMouse(int xless = 0, int yless = 0);

	/** Retrieve mouse X position */
	int getMouseX();

	/** Retrieve mouse Y position */
	int getMouseY();

	/** Function called right before drawing the map */
	virtual void beforeDrawMap();

	/** Function called right after drawing the map */
	virtual void afterDrawMap();

	/** Draws the map (if any) */
	void DrawMap();
};

#ifdef _MSC_VER
#	ifndef __ROENGINE_NO_AUTOIMPORT_LIB__
#		pragma comment(lib, "roengine.lib")
#	endif
#endif

#endif /* __RO_ENGINE_H */
