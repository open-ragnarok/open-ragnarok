/* $Id$ */
#ifndef __ACT_H
#define __ACT_H

#include "object.h"

#include "ro_seq.h"

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
		class MYLIB_DLLAPI Spr {
		public:
			Spr();
			Spr(const Spr&);
			~Spr();

			bool readStream(std::istream&, const s_obj_ver&);
			bool writeStream(std::ostream&, const s_obj_ver&) const;

			Spr& operator = (const Spr&);

#ifdef ROINT_USE_XML
			TiXmlElement *GenerateXML(const s_obj_ver&) const;
#endif
			// DATA //
			int x;
			int y;
			int sprNo;
			unsigned int mirrorOn;
			unsigned int color;
			float xyMag;
			float xMag;
			float yMag;
			unsigned int rot;
			unsigned int type;

			int w;
			int h;
		protected:
			void copyFrom(const Spr&);
		};

		class MYLIB_DLLAPI Pat {
		public:
			Pat();
			Pat(const Pat&);
			~Pat();

			bool readStream(std::istream&, const s_obj_ver&);
			bool writeStream(std::ostream&, const s_obj_ver&) const;

			Spr& operator[] (unsigned int i);
			const Spr& operator[] (unsigned int i) const;

			Pat& operator = (const Pat&);

#ifdef ROINT_USE_XML
			TiXmlElement *GenerateXML(const s_obj_ver&) const;
#endif

		// DATA //
			int pal[2];
			short unk[12];
			unsigned int numspr;
			Seq<Spr> spr;
			int sndNo; // only in version 2.
			int numxxx; // version > 0x0201 (does not exists on 0x0201)
			int ext1;
			int ext_x;
			int ext_y;
			int terminate;

		protected:
			void copyFrom(const Pat&);
		};

		class MYLIB_DLLAPI Act {
		public:
			Act();
			Act(const Act&);
			~Act();

			bool readStream(std::istream&, const s_obj_ver&);
			bool writeStream(std::ostream&, const s_obj_ver&) const;

			Pat& operator[] (unsigned int i);
			const Pat& operator[] (unsigned int i) const;

			Act& operator = (const Act&);

#ifdef ROINT_USE_XML
			TiXmlElement *GenerateXML(const s_obj_ver&) const;
#endif

			// DATA //
			unsigned int patnum;
			Seq<Pat> pat;
		protected:
			void copyFrom(const Act&);
		};

	protected:
		Seq<Act> acts;
		short unk1;
		int unk2;
		int unk3;

		void ClearAll();
	public:
		ACT();
		ACT(const ACT&);
		virtual ~ACT();

		virtual bool readStream(std::istream&);
		virtual bool writeStream(std::ostream&) const;
		const Act& operator[] (const unsigned int&) const;
		unsigned int count() const;
		void Dump(std::ostream&, const std::string& pfx = "") const;

		ACT& operator = (const ACT&);

#ifdef ROINT_USE_XML
		TiXmlElement *GenerateXML(const std::string& name = "", bool utf = true) const;
		TiXmlDocument GenerateXMLDoc(const std::string& name = "", bool utf = true) const;
		bool SaveXML(std::ostream& out, const std::string& name = "", bool utf = true) const;
		bool SaveXML(const std::string& fn, const std::string& name = "", bool utf = true) const;
#endif
	};
}

#endif /* __ACT_H */

