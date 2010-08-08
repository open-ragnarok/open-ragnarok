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
#include "roengine/objects/homunobj.h"
#include "roengine/objects/mobobj.h"

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
	unsigned int m_lag, m_fps, m_elapsed;
;
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
	bool m_shift;
	bool m_moving;

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
	ro::CDir m_cameradir;

	int mousex;
	int mousey;

	/**
	 * The active map.
	 * Will be drawn if not null
	 */
	RswObject* m_map;

	/**
	 * Weather to draw or not to draw the map.
	 * Will only draw if this is true.
	 */
	bool m_drawmap;

	/**
	 * All the actors that the user can currently see
	 */
	std::map<unsigned int, Actor*> m_actors;

	Actor* m_overactor;

	/**
	 * Clears all the actors (useful when changing maps)
	 */
	void clearActors();
	
	std::map<unsigned short, std::string> m_npc_names;
	std::map<unsigned short, std::string> m_job_names;
	std::map<unsigned short, std::string> m_homunculus_names;
	std::map<unsigned short, std::string> m_mercenary_names;
	std::map<unsigned short, std::string> m_mob_names;

	std::map<std::string, std::string> m_mp3_names;
	std::map<unsigned short, std::string> m_itemresource_names;
	std::map<unsigned short, std::string> m_item_names;
	std::map<unsigned short, std::string> m_itemdesc_names;
	std::map<std::string, std::string> m_map_names;

	bool ReadNameTable(const char* fn, std::map<unsigned short, std::string>&);

public:
	ROEngine(const std::string& name = "");
	~ROEngine();

	void Run();

	void ReadIni(const std::string& name = "data.ini");

	/** Event raised when the user left-clicks a map and there is nothing (mob, items, portals, etc) on the clicked place */
	virtual void clickMap(int x, int y);
	virtual void clickMob(int x, int y, MobObj* mob);
	virtual void clickNpc(int x, int y, NpcObj* npc);
	virtual void clickItem(int x, int y);
	virtual void clickPortal(int x, int y, NpcObj* portal);

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

	void Pickup(int x, int y);

};

#ifdef _MSC_VER
#	ifndef __ROENGINE_NO_AUTOIMPORT_LIB__
#		pragma comment(lib, "roengine.lib")
#	endif
#endif

#endif /* __RO_ENGINE_H */
