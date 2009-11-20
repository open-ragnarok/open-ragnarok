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
#ifndef __RO_TYPES_GRF_H
#define __RO_TYPES_GRF_H

#include "../roint_import.h"

#include <string>
#include <iostream>
#include <fstream>

namespace ro {

/**
 * Reads, decompresses and decrypts the grf structure and returns readable data.
 * <b>This currently supports only GRF 2.0 version files.</b>
 *
 * \ingroup ROInterface
 */
class ROINT_DLLAPI GRF {
public:
#pragma pack(push,1)
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
#pragma pack(pop)

	/** Presents information on each file inside of the GRF */
	class ROINT_DLLAPI FileTableItem {
	public:
		FileTableItem();
		FileTableItem(const FileTableItem&);
		~FileTableItem();
		bool readStream(std::istream& ss);

		FileTableItem& operator = (const FileTableItem&);


		char* filename;
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

	/** Saves the file f into the file filename */
	bool save(const std::string& f, const std::string& filename);

	bool fileExists(const std::string& fn) const;

	unsigned int getCount() const;
	std::string getFilename(const unsigned int& i) const;
	const FileTableItem& operator[] (const unsigned int& i) const;
	const FileTableItem& getItem(const unsigned int& i) const;
};

} /* namespace ro */

#endif /* __RO_TYPES_GRF_H */

