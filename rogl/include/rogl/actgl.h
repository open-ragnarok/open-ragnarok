/* $Id$ */
#ifndef __ACTGL_H
#define __ACTGL_H

#include "rogl/globject.h"
#include "rogl/sprgl.h"

#include "ro/types/act.h"
#include "ro/types/spr.h"
#include "ro/types/grf.h"

#include <string>

namespace rogl {

/**
 * Holds the ACT/SPR data pair and draws it accordingly
 */
class ROGL_DLLAPI ActGL : public GLObject {
protected:
	RO::ACT *act;
	SprGL spr;

	const ActGL* ext;

	/**
	 * Draws a cross indicating the spot that the spr should be
	 */
	void ActGL::Cross(float size = 5.0f);

	unsigned int m_action;
	/**
	 * Accumulated delay. Used for calculating next frame.
	 */
	unsigned long m_delay;

	/**
	 * Current frame being drawn.
	 */
	unsigned int m_frame;
public:
	ActGL();
	ActGL(const ActGL&);
	virtual ~ActGL();

	/**
	 * Copy operator
	 */
	ActGL& operator = (const ActGL&);

	void setExt(const ActGL*);
	const ActGL* getExt();

	void setAct(RO::ACT*);
	RO::ACT* getAct();

	void setSpr(const SprGL&);
	SprGL& getSpr();

	bool valid() const;

	/**
	 * Draws this object in the current position
	 * 
	 * @param delay milliseconds passed since last Draw() was called (to calculate animations)
	 */
	void Draw(unsigned long delay);

	void Draw() const;
};

}

#endif /* __ACT_H */
