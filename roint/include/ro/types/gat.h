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
#ifndef __GAT_H
#define __GAT_H

#include "../ro_object.h"

namespace RO {
	/**
	 * Ground Altitude File
	 *
	 * \ingroup ROInterface
	 */
	class ROINT_DLLAPI GAT : public Object {
	public:
#pragma pack(push,1)
		typedef struct strBlock {
			float height[4];
			/**
			 * The type of the block
			 * <pre>
			 * 0 = walkable block
			 * 1 = non-walkable block
			 * 2 = non-walkable water (not snipable)
			 * 3 = walkable water
			 * 4 = non-walkable water (snipable)
			 * 5 = cliff (snipable)
			 * 6 = cliff (not snipable)
			 * Everything else = unknown
			 * </pre>
			 */ 
			unsigned char type;
			unsigned char unk[3];
		};
#pragma pack(pop)
	protected:
		unsigned int m_width, m_height;
		strBlock* m_blocks;
	public:
		GAT();
		~GAT();

		virtual bool readStream(std::istream&);
		bool writeStream(std::ostream&) const;

		unsigned int getWidth() const;
		unsigned int getHeight() const;

		const strBlock& getBlock(const unsigned int& idx) const;
		strBlock& getBlock(const unsigned int& idx);

		const strBlock& getBlock(const unsigned int& x, const unsigned int& y) const;
		strBlock& getBlock(const unsigned int& x, const unsigned int& y);

		const strBlock& operator[] (const unsigned int& idx) const;
		strBlock& operator[] (const unsigned int& idx);
	};
}

#endif /* __GAT_H */

