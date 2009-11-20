/* $Id$ */
/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of The Open Ragnarok Project
    Copyright 2007 - 2009 The Open Ragnarok Team
    For the latest information visit http://www.open-ragnarok.org
    ------------------------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place - Suite 330, Boston, MA 02111-1307, USA, or go to
    http://www.gnu.org/copyleft/lesser.txt.
    ------------------------------------------------------------------------------------
*/
#ifndef __RO_TYPES_ACT_H
#define __RO_TYPES_ACT_H

#include "../ro_object.h"
#include "../ro_arr.h"

namespace ro {

/** Action class.
 *
 * This class holds information on how to correctly draw sprites to create walking characters, 
 * item animation and such.
 * \ingroup ROInterface
 */
class ROINT_DLLAPI ACT : public Object {
public:
	/** Sprite Clip. */
	class ROINT_DLLAPI SprClip {
	public:
		SprClip();
		SprClip(const SprClip&);
		~SprClip();

		bool readStream(std::istream&, const s_obj_ver&);
		bool writeStream(std::ostream&, const s_obj_ver&) const;

		SprClip& operator = (const SprClip&);

#ifdef ROINT_USE_XML
		TiXmlElement *GenerateXML(const s_obj_ver&) const;
#endif
		// DATA //
		int x;
		int y;
		int sprNo; //< -1 for none
		unsigned int mirrorOn; //< mirror image along the vertical axis if non-zero
		unsigned int color; //< (uchar r,g,b,a;)
		float xMag;
		float yMag;
		int angle; //< degrees
		int sprType; //< 0=palette image,1=rgba image
		int w;
		int h;
	protected:
		void copyFrom(const SprClip&);
	};

	/** Motion, represents a frame in an action.
	 * Holds a collection of sprite clips and a collection of attach points.
	 */
	class ROINT_DLLAPI Motion {
	public:
		Motion();
		Motion(const Motion&);
		~Motion();

		bool readStream(std::istream&, const s_obj_ver&);
		bool writeStream(std::ostream&, const s_obj_ver&) const;

		SprClip& operator[] (unsigned int i);
		const SprClip& operator[] (unsigned int i) const;
		unsigned int getClipCount() const;
		const SprClip& getClip(unsigned int clp) const;

		Motion& operator = (const Motion&);

#ifdef ROINT_USE_XML
		TiXmlElement *GenerateXML(const s_obj_ver&) const;
#endif

		// DATA //
		struct AttachPoint {
			int x;
			int y;
			int attr;
		};

		int range1[4]; //< left,top,right,bottom
		int range2[4]; //< left,top,right,bottom
		int eventId; //< -1 for none
		Arr<SprClip> sprClips;
		Arr<AttachPoint> attachPoints;

	protected:
		void copyFrom(const Motion&);
	};

	/** Action. Holds a collection of motions. */
	class ROINT_DLLAPI Action {
	public:
		Action();
		Action(const Action&);
		~Action();

		bool readStream(std::istream&, const s_obj_ver&);
		bool writeStream(std::ostream&, const s_obj_ver&) const;

		Motion& operator[] (unsigned int i);
		const Motion& operator[] (unsigned int i) const;
		unsigned int getMotionCount() const;
		const Motion& getMotion(unsigned int mot) const;

		Action& operator = (const Action&);

#ifdef ROINT_USE_XML
		TiXmlElement *GenerateXML(const s_obj_ver&) const;
#endif

		// DATA //
		Arr<Motion> motions;
	protected:
		void copyFrom(const Action&);
	};

protected:
	void reset();

	struct Event {
		char name[40];
	};

	unsigned char m_reserved[10];
	Arr<Action> m_actions;
	Arr<Event> m_events;
	Arr<float> m_delays;

public:
	/** Basic Contructor */
	ACT();

	/** Copy Constructor */
	ACT(const ACT&);
	virtual ~ACT();

	/** Stream reader implemented from ro::Object */
	virtual bool readStream(std::istream&);

	/** Writes the file to a stream.
		Note that the stream passed must be in binary mode or things will not look good.
	*/
	virtual bool writeStream(std::ostream&) const;
	void Dump(std::ostream&, const std::string& pfx = "") const;

	/** Basic copier */
	ACT& operator = (const ACT&);

	const Action& operator[] (unsigned int act) const;
	unsigned int getActionCount() const;
	const Action& getAction(unsigned int act) const;
	unsigned int getMotionCount(unsigned int act) const;
	const Motion& getMotion(unsigned int act, unsigned int mot) const;
	const char* getEventName(unsigned int evt) const;
	float getDelay(unsigned int act) const;

#ifdef ROINT_USE_XML
	virtual TiXmlElement *GenerateXML(const std::string& name = "", bool utf = true) const;
#endif

};

} /* namespace ro */

#endif /* __RO_TYPES_ACT_H */

