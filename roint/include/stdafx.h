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
#ifndef __STDAFX_H
#define __STDAFX_H

#define TIXML_USE_STL

#ifdef _MSC_VER
#	define USE_LOGSYS // In MSVC we define it here. Other systems, we use the makefile.
#	define _CRT_SECURE_NO_WARNINGS
#	ifndef ROINT_DLL
#		pragma comment(lib, "zlib.lib")
#	else
#		pragma comment(lib, "zlib.dll.lib")
#	endif
#	ifdef USE_LOGSYS
#		pragma comment(lib, "logsys.lib")
#	endif
#endif

#include "ro/roint_settings.h"

#define ROINT_INTERNAL

#include "ro/roint_import.h"
#include "ro/ro.h"

#include "log.h"

#endif /* __STDAFX_H */
