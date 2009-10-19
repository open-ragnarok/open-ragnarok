#ifndef __ROENGINE_ACTOR_H
#define __ROENGINE_ACTOR_H

#include "rogl/actgl.h"
#include "rsw_object.h"

/**
 * Base class for all actors in the game: Chars, Mobs, NPC, etc.
 *
 * Note: Child classes need to implement the Draw() virtual function
 */
class Actor : public GLObject {
public:
	typedef enum {
		UnknownType,
		NpcType,
		CharType,
		PetType,
		HomunculusType,
		MercenaryType
	} Type;

protected:
	/**
	 * The map we are currently in.
	 * This is uses to calculate our position when drawing
	 */
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

	Type m_type;

public:
	/** Actor ID as assigned by the server */
	unsigned int id;

	/** Actor Type as assigned by the server */
	unsigned int type;

	Actor();
	Actor(Type);
	virtual ~Actor();

	virtual bool valid() const;

	void setMap(RswObject*);

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

#endif /* __ROENGINE_ACTOR_H */
