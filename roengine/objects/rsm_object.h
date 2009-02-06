/* $Id$ */
#ifndef __RSM_OBJECT_H
#define __RSM_OBJECT_H

#include "../gl_object.h"
#include "rsm.h"
#include "../texturemanager.h"
#include "../file_manager.h"

#include <vector>

class RsmObject : public GLObject {
protected:
	const RO::RSM* rsm;
	std::vector<unsigned int> textures;

	static void DrawBoundingBox(const RO::RSM::BoundingBox& box);
	void DrawMesh(unsigned int meshid = 0);
	void CalcRotFrame(const RO::RSM::Mesh& mesh, float* Ori, int& time) const;

	int m_time;

public:
	RsmObject(const RO::RSM*);

	bool loadTextures(TextureManager&, FileManager&);

	virtual void Draw();
};

#endif /* __RSM_OBJECT_H */
