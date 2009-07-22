/* $Id$ */
#ifndef __RSM_OBJECT_H
#define __RSM_OBJECT_H

#include "../gl_object.h"
#include "../texturemanager.h"
#include "../file_manager.h"
#include "../base_cache.h"

#include "ro/types/rsm.h"
#include "ro/types/rsw.h"

#include "../frustum.h"

#include <vector>

class RsmObject : public GLObject {
protected:
	const RO::RSM* rsm;
	const RO::RSW::Model* model;
	rogl::Texture::PointerCache textures;

	static void DrawBoundingBox(const RO::RSM::BoundingBox& box);
	void DrawMesh(unsigned int meshid = 0);
	void CalcRotFrame(const RO::RSM::Mesh& mesh, float* Ori, int& time) const;

	int m_time;
	bool is_static;
	unsigned int rsm_gl;

public:
	RsmObject(const RO::RSM*, const RO::RSW::Model* mdl = NULL);
	virtual ~RsmObject();

	bool loadTextures(TextureManager&, FileManager&);
	virtual bool isInFrustum(const Frustum&) const;
	rogl::Texture::Pointer getTexture(unsigned int i);

	virtual void Draw();
};

#endif /* __RSM_OBJECT_H */
