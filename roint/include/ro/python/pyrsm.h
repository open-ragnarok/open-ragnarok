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
#ifndef __RO_PYTHON_PYRSM_H
#define __RO_PYTHON_PYRSM_H

#include "../types/rsm.h"

namespace ro {
extern "C" {

ROINT_DLLAPI RSM* new_RSM();
ROINT_DLLAPI void del_RSM(RSM*);

ROINT_DLLAPI bool read_RSM(RSM*, const char*);
ROINT_DLLAPI void Dump_RSM(const RSM*, const char*);
ROINT_DLLAPI bool write_RSM(const RSM* rsm, const char* fn);
ROINT_DLLAPI const char* getTexture_RSM(const RSM* rsm, int idx);

ROINT_DLLAPI int getMeshCount_RSM(const RSM* rsm);
ROINT_DLLAPI int getTextureCount_RSM(const RSM* rsm);

// RSM::MESH interfaces
/*
ROINT_DLLAPI const RSM::Node::Header* getHeader_Mesh_RSM(const RSM*, int idx);
ROINT_DLLAPI const RSM::Node::Transf* getTransf_Mesh_RSM(const RSM*, int idx);
*/

ROINT_DLLAPI int getMeshVecCount_RSM(const RSM*, int mesh_id);
ROINT_DLLAPI int getMeshTexCount_RSM(const RSM*, int mesh_id);
ROINT_DLLAPI int getMeshTexvCount_RSM(const RSM*, int mesh_id);
ROINT_DLLAPI int getMeshSurfCount_RSM(const RSM*, int mesh_id);

ROINT_DLLAPI const RSM::Face* getMeshSurf_RSM(const RSM*, int mesh_id, int surf_id);
ROINT_DLLAPI const RSM::Vertex* getMeshVec_RSM(const RSM*, int mesh_id, int vec_id);
ROINT_DLLAPI int getMeshTex_RSM(const RSM*, int mesh_id, int tex_id);
ROINT_DLLAPI const RSM::TVertex*  getMeshTexv_RSM(const RSM*, int mesh_id, int texv_id);

} /* extern "C" */
} /* namespace ro */

#endif /* __RO_PYTHON_PYRSM_H */

