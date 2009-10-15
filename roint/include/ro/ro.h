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
#ifndef __RO_H
#define __RO_H
/** \file ro.h
 * This is the main header for the Interface Module.
 * All projects that needs this module, should include this file, and this file only.
 */

/** \mainpage Open-Ragnarok Project
 *
 * \section intro_sec Introduction
 *
 * Welcome to the Open-Ragnarok project. This project has a goal to be a full replacement for the official Ragnarok Client.
 * It consists in 4 main projects. The interface, the GL, the Network and the Client.
 *
 * \section interface_sec The Interface
 *
 * \subsection step1 Step 1: Opening the box
 *  
 * \section gl_sec The Graphics Library
 *
 * \section net_sec The Network
 *
 * \section client_sec The Client
 */

#include "roint_settings.h"

#define GRF_HEADER "Master of Magic"
#define SPR_HEADER "SP"
#define ACT_HEADER "AC"
#define RSM_HEADER "GRSM"
#define RSW_HEADER "GRSW"
#define GND_HEADER "GRGN"
#define GAT_HEADER "GRAT"

#include "roint_import.h"

/** \namespace RO Main namespace for the Interface.
 * 
 */
namespace RO {
extern "C" {
	ROINT_DLLAPI unsigned short Version();
	ROINT_DLLAPI void printVersion();
	ROINT_DLLAPI const char* strVersion();
}
}

#include "ro_strings.h"
#include "constants.h"
#include "ro_object.h"

// the file types
#include "types/grf.h"
#include "types/spr.h"
#include "types/act.h"
#include "types/rsm.h"
#include "types/rsw.h"
#include "types/gnd.h"
#include "types/gat.h"

/**
 * \defgroup ROInterface Interface
 */

#ifdef ROINT_USE_XML
#	include "tinyxml/tinyxml.h"
#endif


#endif // __RO_H
