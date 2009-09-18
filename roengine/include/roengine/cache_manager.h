#ifndef __ROENGINE_CACHE_MASTER_H
#define __ROENGINE_CACHE_MASTER_H

#include "ro_object_cache.h"
#include "gl_object_cache.h"
#include "texturemanager.h"
#include "file_manager.h"

/**
 * Responsible for managing all cache objects of ROEngine
 */
class CacheManager {
protected:
	/** Cache of textures */
	TextureManager m_texturemanager;

	/** Cache of GL Objects */
	GLObjectCache m_gl_objects;

	/** Cache of RO Objects
	 * Primitive objects from GRF file
	 */
	ROObjectCache m_ro_objects;

	/**
	 * Manager to load files from disk/memory
	 */
	FileManager m_filemanager;

public:
	CacheManager();
	virtual ~CacheManager();

	TextureManager& getTextureManager();

	GLObjectCache& getGLObjects();

	ROObjectCache& getROObjects();

	FileManager& getFileManager();
};

#endif /* __ROENGINE_CACHE_MANAGER_H */
