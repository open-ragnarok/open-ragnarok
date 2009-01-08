/* $Id$ */
#include "stdafx.h"

#include "pyrsm.h"
#include <fstream>

namespace RO {
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
		bool ret = rsm->Write(fp);
		fp.close();
		return(ret);
	}

	const char* getTexture_RSM(const RSM* rsm, int idx) {
		return(rsm->getTexture(idx));
	}

	int getMeshCount_RSM(const RSM* rsm) {
		return(rsm->getMeshCount());
	}

	int getTextureCount_RSM(const RSM* rsm) {
		return(rsm->getTextureCount());
	}

	const RSM::Mesh::Header* getHeader_Mesh_RSM(const RSM* rsm, int idx) {
		return(&rsm->getMesh(idx).header);
	}

	const RSM::Mesh::Transf* getTransf_Mesh_RSM(const RSM* rsm, int idx) {
		return(&rsm->getMesh(idx).transf);
	}

	int getMeshVecCount_RSM(const RSM* rsm, int mesh_id) {
		return(rsm->getMesh(mesh_id).vecs.size());
	}

	int getMeshTexCount_RSM(const RSM* rsm, int mesh_id) {
		return(rsm->getMesh(mesh_id).textures.size());
	}

	int getMeshTexvCount_RSM(const RSM* rsm, int mesh_id) {
		return(rsm->getMesh(mesh_id).texv.size());
	}

	int getMeshSurfCount_RSM(const RSM* rsm, int mesh_id) {
		return(rsm->getMesh(mesh_id).surfaces.size());
	}

	const RSM::Surface* getMeshSurf_RSM(const RSM* rsm, int mesh_id, int surf_id) {
		return(&rsm->getMesh(mesh_id).surfaces[surf_id]);
	}

	const RSM::Vec* getMeshVec_RSM(const RSM* rsm, int mesh_id, int vec_id) {
		return(&rsm->getMesh(mesh_id).vecs[vec_id]);
	}

	int getMeshTex_RSM(const RSM* rsm, int mesh_id, int tex_id) {
		return(rsm->getMesh(mesh_id).textures[tex_id]);
	}

	const RSM::Vec* getMeshTexv_RSM(const RSM* rsm, int mesh_id, int texv_id) {
		return(&rsm->getMesh(mesh_id).texv[texv_id]);
	}
}

