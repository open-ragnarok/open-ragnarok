/* $Id: str.h -1 $ */
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
#ifndef __RO_TYPES_STR_H
#define __RO_TYPES_STR_H

#include "../ro_object.h"

namespace ro {

/**
 * Effect's stream class.
 *
 * \ingroup ROInterface
 */
class ROINT_DLLAPI STR : public Object {
public:
#pragma pack(push,1)
	struct Color {
		float r, g, b, a;
		inline operator float* () { return(&r); }
		inline operator const float* () const { return(&r); }
	};

	struct Position {
		float x;
		float y;
		inline operator float* () { return &x; }
		inline operator const float* () const { return &x; }
	};

	struct TVertex {
		float u, v;
		inline operator float* () { return &u; }
		inline operator const float* () const { return &u; }
	};

	struct Rect {
		float x[4];
		float y[4];
	};

	struct Texture {
		char name[128];
	};

	struct Frame {
		unsigned int number;
		unsigned int type;
		Position pos;
		TVertex uv[2];
		TVertex uv2[2];
		Rect rect;
	//	float aniframe;
		float textureid;
		unsigned int texchangetype;
		float texchangedelta;
		float rotation;
		Color color;
	//	BlendMode srcblend;
	//	BlendMode dstblend;
		unsigned int srcblend;
		unsigned int dstblend;
		unsigned int mtpreset;
	};

	struct Layer {
		Arr<Texture> textures;
		Arr<Frame> frames;
	};
#pragma pack(pop)

protected:
	void reset();

	unsigned int m_fps;
	unsigned int m_maxframe;
	char m_reserved[24];
	Arr<Layer> m_layers;
//	unsigned int NumLayer;

public:
	STR();
	virtual ~STR();

	virtual bool readStream(std::istream&);

	const Layer& getLayer(int) const;

	int getLayerCount() const;

	int getMaxFrame() const;

};

} /* namespace ro */

#endif /* __RO_TYPES_STR_H */

