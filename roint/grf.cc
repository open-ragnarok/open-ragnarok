/* $Id$ */
#include "stdafx.h"

#include "grf.h"

#include <fstream>
#include <sstream>
#include <zlib.h>

RO::GRF::GRF() {
	m_opened = false;
	memset((char*)&m_header, 0, sizeof(Header));
}

RO::GRF::~GRF() {
	if (m_opened)
		close();
}

bool RO::GRF::open(const std::string& fn) {
	if (m_opened) {
		std::cerr << "Can't open GRF file. Object already in use. Close it first." << std::endl;
		return(false);
	}

	m_fp.open(fn.c_str(), std::ios_base::binary);
	if (!m_fp.is_open()) {
		std::cerr << "Error opening GRF file " << fn << std::endl;
		return(false);
	}

	m_fp.read((char*)&m_header, sizeof(Header));
	m_filecount = m_header.number2 - m_header.number1 - 7;

#ifdef _DEBUG
	{
	char buf[256];
	buf[0] = 0;
	std::cout << "GRF Header size: " << sizeof(Header) << std::endl;
	std::cout << "Signature: " << m_header.signature << std::endl;
	for (int i = 0; i < 14; i++)
		sprintf(buf, "%s%x", buf, m_header.allowEncryption[i]);
	std::cout << "allowEncryption: " << buf << std::endl
		<< "FileTableOffset: " << m_header.fileTableOffset << std::endl
		<< "File Count: " << m_filecount << std::endl;
	sprintf(buf, "Version: %04x", m_header.version);
	std::cout << buf << std::endl;
	}
#endif

	// Go to the starting offset to read the GRF index.
	m_fp.seekg(m_header.fileTableOffset, std::ios_base::cur);
	if (m_fp.eof()) {
		m_fp.close();
		std::cerr << "Error seeking to position " << (46 + m_header.fileTableOffset) << std::endl;
		return(false);
	}

	m_fp.read((char*)&m_filetableheader, sizeof(unsigned int) * 2);
	m_filetableheader.body = new unsigned char[m_filetableheader.compressedLength];
	m_filetableheader.uncompressedBody = new unsigned char[m_filetableheader.uncompressedLength];
	m_fp.read((char*)m_filetableheader.body, m_filetableheader.compressedLength);
	unsigned long ul = m_filetableheader.uncompressedLength;

	uncompress(m_filetableheader.uncompressedBody, &ul, m_filetableheader.body, m_filetableheader.compressedLength);
	if (ul == 0) {
		std::cerr << "cannot uncompress FileTableHeader" << std::endl;
		return(false);
	}
	if (ul != m_filetableheader.uncompressedLength) {
		std::cerr << "GRF Warning: Uncompressed lengths for FileTableHeader differ!" << std::endl;
		std::cerr << m_filetableheader.uncompressedLength << " / " << ul << std::endl;
	}

	// Read files
	int i, idx;
	m_items = new FileTableItem[m_filecount];
	char buf[512];
	char c;
	std::stringstream ss;
	ss.write((char*)m_filetableheader.uncompressedBody, m_filetableheader.uncompressedLength);

	// std::cout << std::endl;
#ifdef _DEBUG
	int lastporc = -1, iporc;
	float porc;
#endif
	for (i = 0; i < m_filecount; i++) {
		idx = 0;
		c = -1;
		while (c != 0) {
			ss.get(c);
			buf[idx++] = c;
		}
		buf[idx] = 0;
		m_items[i].filename = buf;

		ss.read((char*)&m_items[i].compressedLength, sizeof(unsigned int) * 4 + 1);
		// ss.read((char*)&m_items[i].compressedLengthAligned, sizeof(unsigned int));
		// ss.read((char*)&m_items[i].uncompressedLength, sizeof(unsigned int));
		// ss.get((char&)m_items[i].flags);
		// ss.read((char*)&m_items[i].offset, sizeof(unsigned int));

#ifdef _DEBUG
		porc = ((float)i / (float)m_filecount) * 100.0f;
		iporc = (int)porc;
		if (iporc > lastporc) {
			lastporc = iporc;
			std::cout << "\rTranslated file index " << lastporc << "% ";
		}
#endif
	}
#ifdef _DEBUG
	std::cout << std::endl;
#endif

	m_opened = true;
	return(true);
}

void RO::GRF::close() {
	if (!m_opened) {
		std::cerr << "GRF already closed" << std::endl;
		return;
	}

	delete[] m_filetableheader.body;
	delete[] m_filetableheader.uncompressedBody;
	delete[] m_items;
	if (m_fp.is_open())
		m_fp.close();
	m_opened = false;
}

bool RO::GRF::isOpen() const {
	return(m_opened);
}

bool RO::GRF::write(const std::string& s, std::ostream& out) {
	if (!m_opened)
		return(false);

	// search for file
	for (int i = 0; i < m_filecount; i++) {
		if (m_items[i].filename == s) {
			unsigned char *body;
			unsigned char *uncompressed;
			body = new unsigned char[m_items[i].compressedLength];
			uncompressed = new unsigned char[m_items[i].uncompressedLength];

			unsigned long ul;

			m_fp.seekg(m_items[i].offset + 46);
//#ifdef DEBUG
//			std::cout << "File: " << s << ", Offset: " << m_items[i].offset << "+46 ";
//#endif
			m_fp.read((char*)body, m_items[i].compressedLength);
			ul = m_items[i].uncompressedLength;
			int r;
			if ((r = uncompress(uncompressed, &ul, body, m_items[i].compressedLength)) != Z_OK) {
				std::cerr << "Error uncompressing data ";
				if (r == Z_MEM_ERROR)
					std::cerr << "Z_MEM_ERROR";
				if (r == Z_BUF_ERROR)
					std::cerr << "Z_BUF_ERROR";
				if (r == Z_STREAM_ERROR)
					std::cerr << "Z_STREAM_ERROR";
				std::cerr << std::endl;

				delete[] uncompressed;
				delete[] body;
				return(false);
			}
			if (ul != m_items[i].uncompressedLength)
				std::cerr << "GRF Warning: Uncompressed lengths for file " << m_items[i].filename << " differs!" << std::endl;

			out.write((char*)uncompressed, ul);

			/*{
				char buf[16];
				for (int k = 0; k< 4; k++)
					buf[k] = uncompressed[k];
				printf("0x%02x%02x%02x%02x - %c%c%c%c\n", buf[0], buf[1], buf[2], buf[3], buf[0], buf[1], buf[2], buf[3]);
			}*/

			delete[] body;
			delete[] uncompressed;

			return(true);
		}
	}
	return(false);
}

bool RO::GRF::save(const std::string& s, const std::string& filename) {
	if (!m_opened)
		return(false);

	std::ofstream out(filename.c_str(), std::ios_base::out | std::ios_base::binary);
	bool r = write(s, out);
	out.close();
	return(r);
}

std::string RO::GRF::getFilename(const unsigned int& i) const {
	if (!m_opened)
		return("");

	if (i >= (unsigned int)m_filecount)
		return("");

	return(m_items[i].filename);
}

unsigned int RO::GRF::getCount() const {
	if (!m_opened)
		return(0);

	return(m_filecount);
}
