/* $Id$ */
#ifndef __RSM_OBJECT_H
#define __RSM_OBJECT_H

#include "../gl_object.h"
#include "../texturemanager.h"
#include "../file_manager.h"
#include "../base_cache.h"

#include "rsm.h"
#include "gnd.h"

#include <vector>

class RsmObject : public GLObject {
protected:
	const RO::RSM* rsm;
	const RO::GND* gnd;
	Texture::PointerCache textures;

	static void DrawBoundingBox(const RO::RSM::BoundingBox& box);
	void DrawMesh(unsigned int meshid = 0);
	void CalcRotFrame(const RO::RSM::Mesh& mesh, float* Ori, int& time) const;

	int m_time;
	bool is_static;
	unsigned int rsm_gl;

public:
	RsmObject(const RO::RSM*);
	virtual ~RsmObject();

	bool loadTextures(TextureManager&, FileManager&);

	virtual void Draw();
};

#endif /* __RSM_OBJECT_H */
