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
#ifndef __GND_H
#define __GND_H

#include "../ro_object.h"
#include "../struct_io.h"

namespace RO {
	/**
	 * Ground information
	 *
	 * \ingroup ROInterface
	 */
	class ROINT_DLLAPI GND : public Object {
	public:
#pragma pack(push,1)
		// Structure definitions
		struct strTexture {
			char path[40];
			char unk[40];
		};

		struct strLightmap {
			unsigned char brightness[64];
			unsigned char colorrbg[192];
		};

		struct strTile {
			/** This is the "u" coordinate of the texture */
			float texture_start[4];
			/** This is the "v" coordinate of the texture */
			float texture_end[4];
			unsigned short texture_index;
			unsigned short lightmap;
			unsigned char color[4]; // BGRA -- "A" seems to be ignored by the official client
		};

		struct strCube {
			float height[4];
			int tile_up;
			int tile_side;
			int tile_aside;
		};

		struct strGndHeader {
			unsigned int size_x, size_y;
			unsigned int ratio;
			unsigned int texture_count;
			unsigned int texture_size;
		};

		struct strGridInfo {
			unsigned int x;
			unsigned int y;
			unsigned int cell;
		};
#pragma pack(pop)

	protected:
		strGndHeader m_gndheader;
		strGridInfo m_grid;
		
		strTexture* m_textures;
		strLightmap* m_lightmaps;
		StructIO<strTile> m_tiles;
		strCube* m_cubes;

		unsigned int m_lightmapcount;
		unsigned int m_cubecount;
	public:
		GND();
		virtual ~GND();

		void Clear();

		virtual bool readStream(std::istream&);
		void Dump(std::ostream&) const;

#ifdef ROINT_USE_XML
		virtual TiXmlElement *GenerateXML(const std::string& name = "", bool utf = true) const;
#endif

		/** Returns the number of textures used in this object */
		unsigned int getTextureCount() const;

		/** Returns the number of tiles in this object */
		unsigned int getTileCount() const;

		/** Returns the number of cubes in this object */
		unsigned int getCubeCount() const;

		/** Returns the full texture structure */
		const strTexture& getTexture(const unsigned int& idx) const;
		
		/** Returns the full texture structure */
		strTexture& getTexture(const unsigned int& idx);

		/** Gets the texture name */
		const char* getTextureName(const unsigned int& idx) const;

		/**
		 * Retrieves the tile structure.
		 * @param idx index of the texture to retrieve
		 * @return strTile reference
		 */
		const strTile& getTile(const unsigned int& idx) const;

		/**
		 * Retrieves the tile structure.
		 * @param idx index of the texture to retrieve
		 * @return strTile reference
		 */
		strTile& getTile(const unsigned int& idx);

		const strCube& getCube(const unsigned int& x, const unsigned int& y) const;
		strCube& getCube(const unsigned int& x, const unsigned int& y);

		const strCube& getCube(const unsigned int& idx) const;
		strCube& getCube(const unsigned int& idx);

		unsigned int getWidth() const;
		unsigned int getHeight() const;
	};
}

#endif /* __GND_H */

