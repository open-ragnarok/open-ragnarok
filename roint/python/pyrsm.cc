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
#include "stdafx.h"

#include "ro/python/pyrsm.h"
#include <fstream>

namespace ro {

RSM* new_RSM() {
	return(new RSM());
}

void del_RSM(RSM* rsm) {
	delete(rsm);
}

bool read_RSM(RSM* rsm, const char* fn) {
	return(rsm->read(fn));
}

void Dump_RSM(const RSM* rsm, const char* pfx) {
	rsm->Dump(std::cout, pfx);
}

bool write_RSM(const RSM* rsm, const char* fn) {
	std::ofstream fp(fn, std::ios_base::binary);
	bool ret = rsm->writeStream(fp);
	fp.close();
	return(ret);
}

const char* getTexture_RSM(const RSM* rsm, int idx) {
	return(rsm->getTexture(idx));
}

int getMeshCount_RSM(const RSM* rsm) {
	return(rsm->getNodeCount());
}

int getTextureCount_RSM(const RSM* rsm) {
	return(rsm->getTextureCount());
}

/*
const RSM::Node::Header* getHeader_Mesh_RSM(const RSM* rsm, int idx) {
	return(&rsm->getNode(idx).header);
}

const RSM::Node::Transf* getTransf_Mesh_RSM(const RSM* rsm, int idx) {
	return(&rsm->getNode(idx).transf);
}
*/

int getMeshVecCount_RSM(const RSM* rsm, int mesh_id) {
	return(rsm->getNode(mesh_id).vertices.size());
}

int getMeshTexCount_RSM(const RSM* rsm, int mesh_id) {
	return(rsm->getNode(mesh_id).textures.size());
}

int getMeshTexvCount_RSM(const RSM* rsm, int mesh_id) {
	return(rsm->getNode(mesh_id).tvertices.size());
}

int getMeshSurfCount_RSM(const RSM* rsm, int mesh_id) {
	return(rsm->getNode(mesh_id).faces.size());
}

const RSM::Face* getMeshSurf_RSM(const RSM* rsm, int mesh_id, int surf_id) {
	return(&rsm->getNode(mesh_id).faces[surf_id]);
}

const RSM::Vertex* getMeshVec_RSM(const RSM* rsm, int mesh_id, int vec_id) {
	return(&rsm->getNode(mesh_id).vertices[vec_id]);
}

int getMeshTex_RSM(const RSM* rsm, int mesh_id, int tex_id) {
	return(rsm->getNode(mesh_id).textures[tex_id]);
}

const RSM::TVertex* getMeshTexv_RSM(const RSM* rsm, int mesh_id, int texv_id) {
	return(&rsm->getNode(mesh_id).tvertices[texv_id]);
}

} /* namespace ro */

