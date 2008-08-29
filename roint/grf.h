/* $id$ */
#ifndef __GRF_H
#define __GRF_H

#include "impdecl.h"

#include <string>
#include <iostream>
#include <fstream>

// !!!!!! WARNING !!!!!!
// This currently supports only unencrypted GRF files.

namespace RO {
	/**
	 * Reads, decompresses and decrypts the grf structure and returns readable data
	 */
	class MYLIB_DLLAPI GRF {
	public:
#pragma pack(push)
#pragma pack(1)
		/** Header common to all GRF files */
		typedef struct Header {
			char signature[16];
			unsigned char allowEncryption[14];
			unsigned int fileTableOffset;
			unsigned int number1;
			unsigned int number2;
			unsigned int version;
		};

		struct FileTableHeader {
			unsigned int compressedLength;
			unsigned int uncompressedLength;
			unsigned char* body;
			unsigned char* uncompressedBody;
		};

		struct FileTableItem {
			std::string filename;
			unsigned int compressedLength;
			unsigned int compressedLengthAligned;
			unsigned int uncompressedLength;
			unsigned char flags;
			unsigned int offset;
		};
#pragma pack(pop)

	protected:
		Header m_header;
		bool m_opened;
		int m_filecount;
		std::ifstream m_fp;
		FileTableHeader m_filetableheader;
		FileTableItem *m_items;

	public:
		GRF();
		~GRF();

		bool open(const std::string&);
		void close();
		
		bool isOpen() const;

		/** Reads a file from the GRF and writes into the stream. Returns false if error. */
		bool write(const std::string&, std::ostream&);
	};
}

#endif /* __GRF_H */

