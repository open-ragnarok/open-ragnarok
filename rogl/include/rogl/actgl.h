/* $Id$ */
#ifndef __ACTGL_H
#define __ACTGL_H

#include "rogl/globject.h"
#include "rogl/sprgl.h"

#include "ro/types/act.h"
#include "ro/types/spr.h"
#include "ro/types/grf.h"
#include "ro/constants.h"

#include <string>

namespace rogl {

/**
 * Holds the ACT/SPR data pair and draws it accordingly
 */
class ROGL_DLLAPI ActGL : public GLObject {
protected:
	ro::ACT *act;
	SprGL spr;

	/** External entity */
	ActGL* ext;

	/**
	 * Draws a cross indicating the spot that the spr should be
	 */
	void Cross(float size = 5.0f);

	unsigned short m_action;
	/**
	 * Accumulated delay. Used for calculating next frame.
	 */
	unsigned long m_delay;

	/**
	 * Current frame being drawn.
	 */
	unsigned int m_frame;

	bool m_playing;

public:
	ActGL();
	ActGL(const ActGL&);
	virtual ~ActGL();

	/**
	 * Copy operator
	 */
	ActGL& operator = (const ActGL&);

	void setAction(unsigned short);

	void setExt(ActGL*);
	ActGL* getExt();

	void setAct(ro::ACT*);
	ro::ACT* getAct();

	void setSpr(const SprGL&);
	SprGL& getSpr();

	bool isPlaying();

	/** Check if this act is valid */
	bool valid() const;

	/**
	 * Draws this object in the current position
	 * 
	 * @param delay milliseconds passed since last Draw() was called (to calculate animations)
	 */
	void Draw(unsigned long delay, ro::CDir direction = ro::DIR_N, float z = 0.0f, bool loop = true);

	void Draw() const;
};

}

#endif /* __ACT_H */
