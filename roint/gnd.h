/* $id$ */
#ifndef __GND_H
#define __GND_H

#include "object.h"
#include "struct_io.h"

namespace RO {
	class MYLIB_DLLAPI GND : public Object {
	public:
		// Structure definitions
#pragma pack(push)
#pragma pack(1)
		struct strTexture {
			char path[40];
			char unk[40];
		};

		struct strLightmap {
			unsigned char brightness[64];
			unsigned char colorrbg[192];
		};

		struct strTile {
			float texture_start[4];
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

		virtual bool readStream(std::istream&);
		void Dump(std::ostream&) const;

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

		/**
		 * Retrieves the tile structure based on the xy location.
		 * @param x x coordinate
		 * @param y y coordinate
		 * @return strTile reference
		 */
		const strTile& getTile(const unsigned int& x, const unsigned int& y) const;

		/**
		 * Retrieves the tile structure based on the xy location.
		 * @param x x coordinate
		 * @param y y coordinate
		 * @return strTile reference
		 */
		strTile& getTile(const unsigned int& x, const unsigned int& y);

		const strCube& getCube(const unsigned int& x, const unsigned int& y) const;
		strCube& getCube(const unsigned int& x, const unsigned int& y);

		const strCube& getCube(const unsigned int& idx) const;
		strCube& getCube(const unsigned int& idx);
	};
}

#endif /* __GND_H */

