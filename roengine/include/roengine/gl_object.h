/* $Id$ */
#ifndef __GL_OBJECT_H
#define __GL_OBJECT_H

#include "frustum.h"
#include "sdle/texture.h"
#include "vector.h"

/**
 * Basic drawable object of ROEngine
 */
class GLObject {
protected:
	Vector3f pos;
	float xrot, yrot, zrot;
	float scalex, scaley, scalez;

	virtual void AfterDraw();
	virtual void BeforeDraw();

	bool m_useTexture;
	sdle::Texture m_texture;
	bool m_visible;
	long m_tickdelay;
	bool m_frustum_check;
	RO::CDir cameraDir;
	const Frustum* m_frustum;

public:
	GLObject();
	virtual ~GLObject();

	virtual void Draw() = 0;
	/** Draws the object
	 * @param tickdelay how many ticks have passed since the last render was called
	 */
	void Render(long tickdelay, const Frustum*, RO::CDir CameraLook = RO::DIR_N);
	void setPos(const float&, const float&, const float&);
	void setScale(const float&, const float&, const float&);
	void setRot(const float&, const float&, const float&);
	void setRotX(const float&);
	void setRotY(const float&);
	void setRotZ(const float&);

	Vector3f& getPos();
	const Vector3f& getPos() const;

	void setTexture(sdle::Texture& tex);
	void useTexture(bool);
	void setVisible(bool = true);
	void setFrustumCheck(bool = true);
	bool isVisible() const;
	virtual bool isInFrustum(const Frustum*) const;
};

#endif /* __GL_OBJECT_H */
