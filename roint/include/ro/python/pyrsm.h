/* $Id$ */
#ifndef __PYRSM_H
#define __PYRSM_H

#include "../types/rsm.h"

namespace RO {
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
	ROINT_DLLAPI const RSM::Mesh::Header* getHeader_Mesh_RSM(const RSM*, int idx);
	ROINT_DLLAPI const RSM::Mesh::Transf* getTransf_Mesh_RSM(const RSM*, int idx);

	ROINT_DLLAPI int getMeshVecCount_RSM(const RSM*, int mesh_id);
	ROINT_DLLAPI int getMeshTexCount_RSM(const RSM*, int mesh_id);
	ROINT_DLLAPI int getMeshTexvCount_RSM(const RSM*, int mesh_id);
	ROINT_DLLAPI int getMeshSurfCount_RSM(const RSM*, int mesh_id);

	ROINT_DLLAPI const RSM::Surface* getMeshSurf_RSM(const RSM*, int mesh_id, int surf_id);
	ROINT_DLLAPI const RSM::Vec* getMeshVec_RSM(const RSM*, int mesh_id, int vec_id);
	ROINT_DLLAPI int getMeshTex_RSM(const RSM*, int mesh_id, int tex_id);
	ROINT_DLLAPI const RSM::Vec*  getMeshTexv_RSM(const RSM*, int mesh_id, int texv_id);

}
}

#endif /* __PYRSM_H */

