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
#ifndef __RSW_H
#define __RSW_H

#include "../ro_object.h"

#ifdef ROINT_USE_XML
#	include "rsm.h"
#	include <map>
#endif

namespace RO {
	/**
	 * Resource World
	 * Interface for RSW format objects/files. These are basically maps that users can walk on.
	 *
	 * \ingroup ROInterface
	 */
	class ROINT_DLLAPI RSW : public Object {
	public:
		enum ObjectType {
			ModelType = 1,
			LightType = 2,
			SoundType = 3,
			EffectType = 4
		};

#pragma pack(push,1)
		/** Abstract RSW object. */
		class ROINT_DLLAPI Object {
		protected:
			Object(ObjectType);
		public:
			virtual ~Object();

			static RO::RSW::Object* readStream(std::istream& s, const RO::s_obj_ver& ver);
			bool writeStream(std::ostream& s, const RO::s_obj_ver& ver) const;
			virtual void Dump(std::ostream& s = std::cout, const std::string& pfx = "") const = 0;

			/** Returns the object type */
			ObjectType getType() const;

			/** Check if we are the same type as the parameter */
			bool isType(ObjectType) const;

			/** Returns a copy of this object */
			Object* Copy() const;

		protected:
			ObjectType m_type;
		};

		/** Model RSW object. */
		class ROINT_DLLAPI ModelObject : public Object {
		public:
			ModelObject();
			ModelObject(const ModelObject& obj);
			virtual ~ModelObject();

			bool readData(std::istream& s, const RO::s_obj_ver& ver);
			bool writeData(std::ostream& s, const RO::s_obj_ver& ver) const;
			virtual void Dump(std::ostream& s = std::cout, const std::string& pfx = "") const;

			ModelObject& operator = (const ModelObject& obj);

			char name[40];
			int animType;
			float animSpeed;
			int blockType;
			char modelName[80]; //< RSM filename
			char nodeName[80];
			float pos[3];
			float rot[3];
			float scale[3];
		};

		/** Light source RSW object. */
		class ROINT_DLLAPI LightObject : public Object {
		public:
			LightObject();
			LightObject(const LightObject& obj);
			virtual ~LightObject();

			bool readData(std::istream& s, const RO::s_obj_ver& ver);
			bool writeData(std::ostream& s, const RO::s_obj_ver& ver) const;
			virtual void Dump(std::ostream& s = std::cout, const std::string& pfx = "") const;

			LightObject& operator = (const LightObject& obj);

			char name[80];
			float pos[3];
			int red;
			int green;
			int blue;
			float range;
		};

		/** Sound source RSW object. */
		class ROINT_DLLAPI SoundObject : public Object {
		public:
			SoundObject();
			SoundObject(const SoundObject& obj);
			virtual ~SoundObject();

			bool readData(std::istream& s, const RO::s_obj_ver& ver);
			bool writeData(std::ostream& s, const RO::s_obj_ver& ver) const;
			virtual void Dump(std::ostream& s = std::cout, const std::string& pfx = "") const;

			SoundObject& operator = (const SoundObject& obj);

			char name[80];
			char waveName[80];
			float pos[3];
			float vol;
			int width;
			int height;
			float range;
			float cycle;
		};

		/** Effect RSW object. */
		class ROINT_DLLAPI EffectObject : public Object {
		public:
			EffectObject();
			EffectObject(const EffectObject& obj);
			virtual ~EffectObject();

			bool readData(std::istream& s, const RO::s_obj_ver& ver);
			bool writeData(std::ostream& s, const RO::s_obj_ver& ver) const;
			virtual void Dump(std::ostream& s = std::cout, const std::string& pfx = "") const;

			EffectObject& operator = (const EffectObject& obj);

			char name[80];
			float pos[3];
			int type;
			float emitSpeed;
			float param[4];
		};

		struct Ground {
			int top;
			int bottom;
			int left;
			int right;
		};

		struct Light {
			int longitude; //< degrees
			int latitude; //< degrees
			float diffuse[3]; //< color
			float ambient[3]; //< color
			float ignored; //< ignored, float?
		};

		struct Water {
			float level;
			int type;
			float waveHeight;
			float waveSpeed;
			float wavePitch;
			int animSpeed;
		};

		struct QuadTreeNode {
			float max[3];
			float min[3];
			float halfSize[3];
			float center[3];
			unsigned int child[4]; //< index of child node (generated), 0 for no child
		};
#pragma pack(pop)

	protected:
		void reset();
		void readQuadTree(std::istream& s, unsigned int level, unsigned int& i);

		char m_iniFile[40];
		char m_gndFile[40];
		char m_gatFile[40];
		char m_scrFile[40];
		Water m_water;
		Light m_light;
		Ground m_ground;
		Arr<Object*> m_objects;
		Arr<QuadTreeNode> m_quadTree; //< 0 or 1365 entries (4^0 + 4^1 + 4^2 + 4^3 + 4^4 + 4^5, quadtree with 6 levels, depth-first ordering)

	public:
		const char* getIniFile() const;
		const char* getGndFile() const;
		const char* getGatFile() const;
		const char* getScrFile() const;
		const Water& getWater() const;
		const Light& getLight() const;
		const Ground& getGround() const;

		unsigned int getObjectCount() const;
		const Object* getObject(unsigned int obj) const;
		const Object* operator[] (unsigned int obj) const;
		const ModelObject* getModelObject(unsigned int obj) const;
		const LightObject* getLightObject(unsigned int obj) const;
		const SoundObject* getSoundObject(unsigned int obj) const;
		const EffectObject* getEffectObject(unsigned int obj) const;
		bool isType(unsigned int obj, ObjectType t) const;

		RSW();
		RSW(const RSW& rsw);
		virtual ~RSW();

		virtual bool readStream(std::istream& s);
		/** Write the RSW data to a stream */
		virtual bool writeStream(std::ostream& s) const;
		virtual void Dump(std::ostream& o = std::cout, const std::string& pfx = "") const;

		RSW& operator = (const RSW&);

#ifdef ROINT_USE_XML
		virtual TiXmlElement *GenerateXML(const std::string& name = "", bool utf = true) const;

		TiXmlElement *GenerateFullXML(const std::map<std::string, RSM*>, const std::string& name = "", bool utf = true) const;
		TiXmlDocument GenerateFullXMLDoc(const std::map<std::string, RSM*>, const std::string& name = "", bool utf = true) const;
		bool SaveFullXML(const std::map<std::string, RSM*>, std::ostream& out, const std::string& name = "", bool utf = true) const;
		bool SaveFullXML(const std::map<std::string, RSM*>, const std::string& fn, const std::string& name = "", bool utf = true) const;

		TiXmlElement *GenerateFullXML(const std::map<std::string, RSM>, const std::string& name = "", bool utf = true) const;
		TiXmlDocument GenerateFullXMLDoc(const std::map<std::string, RSM>, const std::string& name = "", bool utf = true) const;
		bool SaveFullXML(const std::map<std::string, RSM>, std::ostream& out, const std::string& name = "", bool utf = true) const;
		bool SaveFullXML(const std::map<std::string, RSM>, const std::string& fn, const std::string& name = "", bool utf = true) const;
#endif
	};
}

#endif /* __RSW_H */
