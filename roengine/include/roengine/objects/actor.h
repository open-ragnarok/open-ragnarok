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
		MercenaryType,
		MobType,
		ItemType
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
//	float speed;
	unsigned short speed;
//	unsigned short moveticks;

	bool m_visible;

	/** The direction we are facing */
	ro::CDir m_dir;

	Type m_type;

	rogl::ActGL m_shadowact;
	bool shadowLoaded;
	rogl::ActGL m_emotionact;
	bool emotionLoaded;
	int m_emotion;

public:
	/** Actor ID as assigned by the server */
	unsigned int id;

	/** Actor Type as assigned by the server */
	unsigned int typeID;

	Actor();
	Actor(Type);
	virtual ~Actor();

	virtual bool valid() const;

	void setMap(RswObject*);

	float getPositionX() const;
	float getPositionY() const;
	float getDestinationX() const;
	float getDestinationY() const;

	void setDirection(ro::CDir);
	void setSpeed(unsigned short);
	virtual void setAction(unsigned short);

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

	void setVisible(bool);
	bool isVisible() const;
	Type getType() const;

	void setEmotion(int emotion);

	bool openAct(CacheManager& cache, std::string name, rogl::ActGL& actor);
};

#endif /* __ROENGINE_ACTOR_H */
