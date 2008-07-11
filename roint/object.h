/* $id$ */
#ifndef __OBJECT_H
#define __OBJECT_H

#include <iostream>
#include <ostream>
#include <istream>
#include <string>

#include "impdecl.h"

namespace RO {
	union s_obj_ver {
		short sver;
		struct _cver {
			unsigned char major;
			unsigned char minor;
		} cver;
	};

	class MYLIB_DLLAPI Object {
	public:
		Object();
		virtual ~Object();

		virtual bool read(const std::string&);
		virtual bool readStream(std::istream&) = 0;
		bool isValid() const;
		const s_obj_ver* getVersion() const;

	protected:
		unsigned char magicSize;
		char magic[4];
		//unsigned char major_ver, minor_ver;
		s_obj_ver m_version;
		bool m_valid;

		bool IsCompatibleWith(unsigned char major_ver, unsigned char minor_ver) const;
		bool IsCompatibleWith(short ver) const;
		bool readHeader(std::istream&);
		bool writeHeader(std::ostream&) const;
		bool checkHeader(const std::string&) const;
	};
}

#endif /* __OBJECT_H */

