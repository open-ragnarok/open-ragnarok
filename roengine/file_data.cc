/* $Id$ */
#include "stdafx.h"

#include "file_data.h"

FileData::FileData() : DynamicBlobChar() {

}

FileData::FileData(const FileData& o) : DynamicBlobChar(o) {
}

FileData::FileData(const char* data, unsigned int size) : DynamicBlobChar(size) {
	memcpy(this->buffer, data, size);
}

bool FileData::write(std::ostream& os) const {
	os.write((char*)this->buffer, dataSize);
	return(true);
}

FileData& FileData::operator = (const FileData& o) {
	setSize(o.dataSize);
	memcpy(buffer, o.buffer, dataSize);

	return(*this);
}

