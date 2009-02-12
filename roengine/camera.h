#ifndef __CAMERA_H
#define __CAMERA_H

#include "vector.h"

class Camera {
protected:
	Vector3f eye;
	Vector3f dest;
public:
	Camera();
	~Camera();

	void Rotate(float f);
	void Translate(const Vector3f&);
	void TranslateX(const float&);
	void TranslateY(const float&);
	void TranslateZ(const float&);
	void setEye(const Vector3f&);
	void setDest(const Vector3f&);

	void Look() const;
};

#endif /* __CAMERA_H */