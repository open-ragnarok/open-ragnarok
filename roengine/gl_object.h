/* $Id$ */
#ifndef __GL_OBJECT_H
#define __GL_OBJECT_H

class GLObject {
protected:
	float x, y, z;
	float xrot, yrot, zrot;
	float scalex, scaley, scalez;

	virtual void AfterDraw();
	virtual void BeforeDraw();

	bool m_useTexture;
	unsigned int m_textureId;
	bool m_visible;
	long m_tickdelay;

public:
	GLObject();
	virtual ~GLObject();

	virtual void Draw() = 0;
	void Render(long tickdelay = 0);
	void setPos(const float&, const float&, const float&);
	void setScale(const float&, const float&, const float&);
	void setRot(const float&, const float&, const float&);
	void setRotX(const float&);
	void setRotY(const float&);
	void setRotZ(const float&);

	void setTexture(const unsigned int& textureId);
	void useTexture(bool);
	void setVisible(bool = true);
	bool isVisible() const;
};

#endif /* __GL_OBJECT_H */