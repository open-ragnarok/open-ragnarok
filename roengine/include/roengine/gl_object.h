/* $Id$ */
#ifndef __GL_OBJECT_H
#define __GL_OBJECT_H

#include "frustum.h"
#include "rogl/texture.h"
#include "vector.h"

class GLObject {
protected:
	Vector3f pos;
	float xrot, yrot, zrot;
	float scalex, scaley, scalez;

	virtual void AfterDraw();
	virtual void BeforeDraw();

	bool m_useTexture;
	rogl::Texture::Pointer m_texture;
	bool m_visible;
	long m_tickdelay;
	bool m_frustum_check;
	Vector3f cameraDir;

public:
	GLObject();
	virtual ~GLObject();

	virtual void Draw() = 0;
	void Render(long tickdelay = 0, const Vector3f& CameraLook = Vector3f::UNIT_Z);
	void setPos(const float&, const float&, const float&);
	void setScale(const float&, const float&, const float&);
	void setRot(const float&, const float&, const float&);
	void setRotX(const float&);
	void setRotY(const float&);
	void setRotZ(const float&);

	float getAngle(const Vector3f&) const;

	Vector3f& getPos();
	const Vector3f& getPos() const;

	void setTexture(rogl::Texture::Pointer& tex);
	void useTexture(bool);
	void setVisible(bool = true);
	void setFrustumCheck(bool = true);
	bool isVisible() const;
	virtual bool isInFrustum(const Frustum&) const;
};

#endif /* __GL_OBJECT_H */
