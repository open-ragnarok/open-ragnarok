#ifndef __ACT_OBJECT_H
#define __ACT_OBJECT_H


#include "../full_act.h"
#include "../gl_object.h"

class ActObject : public GLObject, public FullAct {
protected:
	unsigned int curframe;
	int m_time;
	int curaction;
	RO::CDir direction;

	void Billboard();
	void Window(float x, float y, const rogl::Texture::Pointer& tex, bool mirrorX = false, bool mirrorY = false);
	virtual void DrawAct(const RO::ACT::Pat& pat, rogl::Texture::Pointer& t);

public:
	ActObject();
	ActObject(const ActObject&);
	ActObject(const RO::ACT*, const rogl::Texture::PointerCache&);
	virtual ~ActObject();

	virtual void Draw();
};

#endif /* __ACT_OBJECT_H */