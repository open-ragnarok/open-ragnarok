/* $Id$ */
#ifndef __GRF_H
#define __GRF_H

#include "impdecl.h"

#include <string>
#include <iostream>
#include <fstream>

namespace RO {
	/**
	 * Reads, decompresses and decrypts the grf structure and returns readable data.
	 * <b>This currently supports only GRF 2.0 version files.</b>
	 *
	 * \ingroup ROInterface
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

		/** Holds a sequence of compressed (and uncompressed) FileTableItem structures */
		struct FileTableHeader {
			unsigned int compressedLength;
			unsigned int uncompressedLength;
			unsigned char* body;
			unsigned char* uncompressedBody;
		};

		/** Presents information on each file inside of the GRF */
		struct FileTableItem {
			std::string filename;
			int compressedLength;
			int compressedLengthAligned;
			int uncompressedLength;
			/**
			 * Bitmask indicating what this file is
			 * <pre>
			 * 0x01 -> File
			 * 0x02 -> MIXCRYPT
			 * 0x03 -> DES
			 * 0x05 -> ??
			 * </pre>
			 *
			 * Source: OpenKore project
			 */
			char flags;
			int offset;
			int cycle; // for DES Decoding purposes
		};
#pragma pack(pop)

	protected:
		Header m_header;
		bool m_opened;
		int m_filecount;
		std::ifstream m_fp;
		FileTableHeader m_filetableheader;

		/** List of files contained in the GRF */
		FileTableItem *m_items;

	private:
		unsigned long m_seekstart;

	public:
		GRF();
		~GRF();

		bool open(const std::string&);
		void close();
		
		bool isOpen() const;

		/** Reads a file from the GRF and writes into the stream. Returns false if error. */
		bool write(const std::string&, std::ostream&);
		
		bool save(const std::string&, const std::string& filename);

		unsigned int getCount() const;
		std::string getFilename(const unsigned int& i) const;
		const FileTableItem& operator[] (const unsigned int& i) const;
		const FileTableItem& getItem(const unsigned int& i) const;
	};
}

#endif /* __GRF_H */

