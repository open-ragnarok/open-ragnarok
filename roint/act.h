/* $Id$ */
#ifndef __ACT_H
#define __ACT_H

#include "object.h"

namespace RO {

	/** Action class.
	 *
	 * This class holds information on how to correctly draw sprites to create walking characters, 
	 * item animation and such.
	 * \ingroup ROInterface
	 */
	class MYLIB_DLLAPI ACT : public Object {
	public:
		// ACT Strutures
#pragma pack(push)
#pragma pack(1)
		struct Spr {
			int x;  // 0x0101 -- everyone have these
			int y;  // 0x0101 -- everyone have these
			int sprNo;  // 0x0101 -- everyone have these
			unsigned int mirrorOn; // 0x0101 -- everyone have these
			unsigned int color; // 0x0201 -- every version 2 have this -- is this a RGBA value?
			float xyMag; // 0x0201 -- on 0x0201 and 0x0203 (what about 0x0202? assumming yes)

			float xMag; // 0x0204 -- on 0x0204 and 0x0205 instead of xyMag
			float yMag; // 0x0204 -- on 0x0204 and 0x0205 instead of xyMag

			unsigned int rot; // 0x0201 -- every version 2 have this
			unsigned int type; // 0x0201 -- every version 2 have this

			int w; // 0x0205 -- only on 0x0205
			int h; // 0x0205 -- only on 0x0205
		};

		struct Pat {
			int pal[2];
			short unk[12];
			unsigned int numspr;
			Spr* spr;
			int sndNo; // only in version 2.
			int numxxx; // version > 0x0201 (does not exists on 0x0201)
			int ext1;
			int ext_x;
			int ext_y;
			int terminate;
		};

		struct Act {
			unsigned int patnum;
			Pat* pat;
		};
#pragma pack(pop)

	protected:
		unsigned short actCount;
		Act* acts;
		short unk1;
		int unk2;
		int unk3;

		void readAct(std::istream&, Act*);
		void readSpr(std::istream&, Spr*);

	public:
		ACT();
		ACT(const ACT&);
		virtual ~ACT();

		virtual bool readStream(std::istream&);
		const Act* operator[] (const unsigned int&) const;
		unsigned int count() const;
		void Dump(std::ostream&, const std::string& pfx = "") const;

		ACT& operator = (const ACT&);

#ifdef ROINT_USE_XML
		TiXmlElement *GenerateXML(const Spr&) const;

		TiXmlElement *GenerateXML(const std::string& name = "", bool utf = true) const;
		TiXmlDocument GenerateXMLDoc(const std::string& name = "", bool utf = true) const;
		bool SaveXML(std::ostream& out, const std::string& name = "", bool utf = true) const;
		bool SaveXML(const std::string& fn, const std::string& name = "", bool utf = true) const;
#endif
	};
}

#endif /* __ACT_H */

