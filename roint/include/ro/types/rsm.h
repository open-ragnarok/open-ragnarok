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
#ifndef __RO_TYPES_RSM_H
#define __RO_TYPES_RSM_H

#include "../ro_object.h"
#include "../struct_io.h"

namespace ro {

/**
 * Resource Model.
 *
 * Holds information about a 3D object.
 *
 * \ingroup ROInterface
 */
class ROINT_DLLAPI RSM : public Object {
public:
	// RSM Internal Structures
#pragma pack(push,1)
	struct Texture {
		char name[40];
	};

	struct Vertex {
		float x;
		float y;
		float z;
		inline operator float* () { return &x; }
		inline operator const float* () const { return &x; }
	};

	struct TVertex {
		unsigned int color;
		float u, v;
		inline operator float* () { return &u; }
		inline operator const float* () const { return &u; }
	};

	/** Triangle face */
	struct Face {
		unsigned short vertidx[3];
		unsigned short tvertidx[3];
		unsigned short texid;
		unsigned short __padding;
		int twoSide;
		int smoothGroup;
	};

	struct PosKeyframe {
		int frame;
		float px;
		float py;
		float pz;
		inline operator float* () { return &px; }
		inline operator const float* () const { return &px; }
	};

	struct RotKeyframe {
		int frame;
		float qx;
		float qy;
		float qz;
		float qw;
		inline operator float* () { return &qx; }
		inline operator const float* () const { return &qx; }
	};

	struct VolumeBox {
		Vertex size;
		Vertex pos;
		Vertex rot;
		int flag; //< if (flag != 0) size += {3,0,3};
	};

	class ROINT_DLLAPI Node {
	public:
		char name[40];
		char parentname[40];
		Arr<int> textures; //< texture indexes
		float offsetMT[12]; //< 3x4 matrix that identifies the axis and origin of this node
		Vertex pos;
		float rotangle; //< angle around the axis of rotation in radians
		Vertex rotaxis; //< axis of rotation
		Vertex scale;
		Arr<Vertex> vertices;
		Arr<TVertex> tvertices;
		Arr<Face> faces;
		Arr<PosKeyframe> posKeyframes;
		Arr<RotKeyframe> rotKeyframes;

		Node();
		Node(const Node&);
		~Node();

		Node& operator = (const Node&);

		void reset();
		bool readStream(std::istream& s, const s_obj_ver& ver);
		bool writeStream(std::ostream& s, const s_obj_ver& ver) const;
		void Dump(std::ostream& out, const std::string& prefix) const;
	};
#pragma pack(pop)

protected:
	void reset();

	int m_animLen;
	int m_shadeType;
	unsigned char m_alpha;
	char m_reserved[16];
	Arr<Texture> m_textures;
	char m_mainNode[40];
	Arr<Node> m_nodes;
	Arr<VolumeBox> m_volumeBoxes;

public:
	RSM();
	RSM(const RSM&);
	virtual ~RSM();

	RSM& operator = (const RSM&);

	virtual bool readStream(std::istream&);
	bool writeStream(std::ostream& s) const;

	void Dump(std::ostream& out, const std::string& = "") const;
#ifdef ROINT_USE_XML
	virtual TiXmlElement *GenerateXML(const std::string& name = "", bool utf = true) const;
#endif

	int getAnimLen() const;
	int getShadeType() const;
	unsigned char getAlpha() const;
	const char* getMainNode() const;

	unsigned int getNodeCount() const;
	const Node& getNode(unsigned int idx) const;
	const Node& operator[] (unsigned int idx) const;
	const Node* findNode(const char* name) const;

	unsigned int getTextureCount() const;
	const char* getTexture(unsigned int idx) const;

	unsigned int getVolumeBoxCount() const;
	const VolumeBox& getVolumeBox(unsigned int idx) const;
};

} /* namespace ro */

#endif /* __RO_TYPES_RSM_H */

