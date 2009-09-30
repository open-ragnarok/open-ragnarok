/* $Id$ */
/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of The Open Ragnarok Project
    Copyright 2007 - 2009 The Open Ragnarok Team
    For the latest information visit http://www.open-ragnarok.org
    ------------------------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place - Suite 330, Boston, MA 02111-1307, USA, or go to
    http://www.gnu.org/copyleft/lesser.txt.
    ------------------------------------------------------------------------------------
*/
#ifndef __ROINT_IMPORT_H
#define __ROINT_IMPORT_H

// The following will ensure that we are exporting our C++ classes when 
// building the DLL and importing the classes when build an application 
// using this DLL.

#include "roint_settings.h"

#ifndef __WINDOWS__
#	if defined(WIN32) || defined(WIN64) || defined(_WINDOWS) || defined(_WIN32) || defined(_WIN64)
#		define __WINDOWS__
#	endif
#endif

#ifndef _MSC_VER
#	define ROINT_DLLAPI
#else
#	ifndef ROINT_DLL
#		define ROINT_DLLAPI
#	else
#		ifdef ROINT_INTERNAL
#			define ROINT_DLLAPI  __declspec( dllexport )
#		else
#			define ROINT_DLLAPI  __declspec( dllimport )
#		endif
#	endif
#endif

// Use XML?
#ifdef ROINT_USE_XML
#	define TIXML_USE_STL
#	include "tinyxml/tinyxml.h"
#endif

#endif /* __ROINT_IMPORT_H */
