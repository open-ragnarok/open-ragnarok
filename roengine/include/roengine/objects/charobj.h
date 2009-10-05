#ifndef __ROENGINE_CHAROBJ_H
#define __ROENGINE_CHAROBJ_H

#include "rogl/actgl.h"
#include "roengine/cache_manager.h"

#include "rsw_object.h"

class CharObj : public GLObject {
protected:
	rogl::ActGL m_bodyact;
	rogl::ActGL m_headact;

	RO::CJob m_job;
	RO::CSex m_sex;
	unsigned short m_hair;

	RswObject* m_map;

	/** Current action to draw */
	unsigned short m_act;

	/** Current frame in the action */
	unsigned short m_frame;

	/** Current map position */
	float map_x, map_y;

	/** Current destination */
	float dest_x, dest_y;

	/** The speed */
	float speed;

	/** The direction we are facing */
	RO::CDir m_dir;

public:
	CharObj();
	virtual ~CharObj();

	virtual void Draw();

	bool valid() const;

	void setMap(RswObject*);

	bool open(CacheManager& cache, RO::CJob job, RO::CSex sex = RO::S_MALE);

	float getPositionX() const;
	float getPositionY() const;
	float getDestinationX() const;
	float getDestinationY() const;

	/**
	 * Setup the current position.
	 * It teleports the char to the given point
	 */
	void setPos(float x, float y);

	/**
	 * Sets the current destination.
	 * The character will move to the position with the given speed.
	 */
	void setDest(float x, float y);
};

#endif /* __ROENGINE_CHAROBJ_H */
