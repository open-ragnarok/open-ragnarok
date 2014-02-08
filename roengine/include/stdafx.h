/* $Id: stdafx.h 61 2009-07-22 14:20:16Z sergio $ */
#ifndef __STDAFX_H
#define __STDAFX_H

#ifdef _MSC_VER
#	include <windows.h>
#endif
#include <SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "sdle/sdl_engine.h"

#include "ro/ro.h"
#include "rogl/rogl.h"

// Ugly hack to avoid some work, TODO: rework this
typedef Uint16 SDLKey;

#endif /* __STDAFX_H */
