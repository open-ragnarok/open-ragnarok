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
		/// Lightmap, represents the shadow projected on a surface.
		struct Lightmap {
			unsigned char brightness[8][8];
			struct {
				unsigned char r;
				unsigned char g;
				unsigned char b;
				inline operator unsigned char* () { return &r; }
				inline operator const unsigned char* () const { return &r; }
			} color[8][8];
		};

		/// Surface, represents the visual aspect of a quad.
		/// The vertices to use depend on which side of the cell is using the surface.
		/// top = T0->T1->T2->T3
		/// front = T2->T3->F0->F1
		/// right = T3->T1->R2->R0
		/// 2---3
		/// | F | (cell y+1)
		/// 0---1
		/// 2---3 2---3
		/// | T | | R | (cell x+1)
		/// 0---1 0---1
		struct Surface {
			float u[4]; //< west->east, south->north ordering; 0=left 1=right
			float v[4]; //< west->east, south->north ordering; 0=up 1=down
			short textureId; //< -1 for none
			unsigned short lightmapId; //< -1 for none?
			struct {
				unsigned char b;
				unsigned char g;
				unsigned char r;
				unsigned char a;
				inline operator unsigned char* () { return &b; }
				inline operator const unsigned char* () const { return &b; }
			} color;//< BGRA -- "A" seems to be ignored by the official client
		};

		struct Cell {
			float height[4]; //< west->east, south->north ordering
			int topSurfaceId; //< -1 for none
			int frontSurfaceId; //< -1 for none
			int rightSurfaceId; //< -1 for none
		};
#pragma pack(pop)

	protected:
		void reset();

		unsigned int m_width;
		unsigned int m_height;
		float m_zoom;

		unsigned int m_nTextures;
		unsigned int m_textureSize;
		char* m_textures;

		Arr<Lightmap> m_lightmaps;
		Arr<Surface> m_surfaces;
		Arr<Cell> m_cells; //< west->east, south->north ordering

	public:
		GND();
		virtual ~GND();

		virtual bool readStream(std::istream& s);
		bool writeStream(std::ostream& s) const;
		void Dump(std::ostream& s) const;

#ifdef ROINT_USE_XML
		virtual TiXmlElement *GenerateXML(const std::string& name = "", bool utf = true) const;
#endif

		unsigned int getWidth() const;
		unsigned int getHeight() const;
		float getZoom() const;

		/** Returns the number of textures used in this object */
		unsigned int getTextureCount() const;

		/** Gets the texture name */
		const char* getTexture(unsigned int idx) const;

		/** Returns the number of lightmaps in this object */
		unsigned int getLightmapCount() const;

		/** Gets the lightmap. */
		const Lightmap& getLightmap(unsigned int idx) const;

		/** Number of surfaces. */
		unsigned int getSurfaceCount() const;

		/** Gets the surface. */
		const Surface& getSurface(unsigned int idx) const;

		/** Number of cells. */
		unsigned int getCellCount() const;

		/** Gets the cell. */
		const Cell& operator [] (unsigned int idx) const;
		const Cell& getCell(unsigned int idx) const;
		const Cell& getCell(unsigned int cellx, unsigned int celly) const;

	};
}

#endif /* __GND_H */

