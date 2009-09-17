#include "stdafx.h"

#include "roengine/cache_manager.h"

CacheManager::CacheManager() {}

CacheManager::~CacheManager() {}

TextureManager& CacheManager::getTextureManager() { return(m_texturemanager); }

GLObjectCache& CacheManager::getGLObjects() { return(m_gl_objects); }

ROObjectCache& CacheManager::getROObjects() { return(m_ro_objects); }

FileManager& CacheManager::getFileManager() { return(m_filemanager); }