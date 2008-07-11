/* $id$ */
#ifndef __PYRSM_H
#define __PYRSM_H

#include "../rsm.h"

namespace RO {
extern "C" {
	MYLIB_DLLAPI RSM* new_RSM();
	MYLIB_DLLAPI void del_RSM(RSM*);

	MYLIB_DLLAPI bool read_RSM(RSM*, const char*);
	MYLIB_DLLAPI void Dump_RSM(const RSM*, const char*);
	MYLIB_DLLAPI bool write_RSM(const RSM* rsm, const char* fn);
	MYLIB_DLLAPI const char* getTexture_RSM(const RSM* rsm, int idx);

	MYLIB_DLLAPI int getMeshCount_RSM(const RSM* rsm);
	MYLIB_DLLAPI int getTextureCount_RSM(const RSM* rsm);

	// RSM::MESH interfaces
	MYLIB_DLLAPI const RSM::Mesh::Header* getHeader_Mesh_RSM(const RSM*, int idx);
	MYLIB_DLLAPI const RSM::Mesh::Transf* getTransf_Mesh_RSM(const RSM*, int idx);
}
}

#endif /* __PYRSM_H */

