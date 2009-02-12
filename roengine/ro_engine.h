/* $Id$ */
#ifndef __RO_ENGINE_H
#define __RO_ENGINE_H

#include "sdl_engine.h"
#include "ro_object_cache.h"
#include "gl_object_cache.h"
#include "texturemanager.h"
#include "file_manager.h"
#include "frustum.h"
#include "camera.h"

#include "ro.h"
#include "rogl.h"
#include "sdl.h"

class ROEngine : public SDLEngine {
protected:
	TextureManager m_texturemanager;
	GLObjectCache m_gl_objects;
	ROObjectCache m_ro_objects;
	FileManager m_filemanager;
	Frustum m_frustum;

	bool m_quit;

	virtual void HandleKeyboard();
	virtual void BeforeRun();
	virtual void AfterRun();
	virtual void BeforeDraw();
	virtual void AfterDraw();

	long lastTick;
	long tickDelay;

	Camera cam;

public:
	ROEngine(const std::string& name = "");

	~ROEngine();
	void Run();

	void ReadIni(const std::string& name = "data.ini");

	virtual void evtQuit();

};

#ifndef __ROENGINE_NO_AUTOIMPORT_LIB__
#	pragma comment(lib, "roengine.lib")
#endif

#endif /* __RO_ENGINE_H */