/* $Id$ */
#ifndef __GAT_H
#define __GAT_H

#include "ro_object.h"

namespace RO {
	/**
	 * Ground Altitude File
	 *
	 * \ingroup ROInterface
	 */
	class ROINT_DLLAPI GAT : public Object {
	public:
#pragma pack(push)
#pragma pack(1)
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

