/* $Id$ */
#ifndef __FRUSTUM_H
#define __FRUSTUM_H
// Thanks to http://robertmarkmorley.com/2008/11/16/frustum-culling-in-opengl/

#include "ro/types/rsm.h"
#include "ro/types/rsw.h"

#include "vector.h"

class Frustum {
protected:
	float frustum[6][4];
public:
	Frustum();
	~Frustum();

	void Calculate();

	bool VectorVisible(const Vector3f&) const;
	bool PointVisible(float x, float y, float z) const;
	bool BoxVisible(float x, float y, float z, float width, float height, float length) const;
	bool ModelVisible(const RO::RSM* rsm, const RO::RSW::Model* model) const;
};

#endif /* __FRUSTUM_H */