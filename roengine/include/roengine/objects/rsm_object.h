/* $Id$ */
#ifndef __RSM_OBJECT_H
#define __RSM_OBJECT_H

#include "../gl_object.h"
#include "../cache_manager.h"

#include "ro/types/rsm.h"
#include "ro/types/rsw.h"

#include "../frustum.h"

#include "../texturecache.h"

#include <vector>

class RsmObject : public GLObject {
protected:
	const ro::RSM* rsm;
	const ro::RSW::ModelObject* model;
	TextureCache textures;

	void DrawBoundingBox() const;
	void DrawMesh(unsigned int meshid = 0);
	void CalcRotFrame(const ro::RSM::Node& node, float* Ori, int& frame) const;

	int m_time;
	bool is_static;
	unsigned int rsm_gl;

public:
	RsmObject(const ro::RSM*, const ro::RSW::ModelObject* mdl = NULL);
	virtual ~RsmObject();

	bool loadTextures(CacheManager&);
	virtual bool isInFrustum(const Frustum&) const;
	sdle::Texture getTexture(unsigned int i);

	virtual void Draw();
};

#endif /* __RSM_OBJECT_H */
