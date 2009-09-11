/* $Id$ */
#include "stdafx.h"

#include "roengine/file_data.h"

FileData::FileData() : DynamicBlob() {

}

FileData::FileData(const FileData& o) : DynamicBlob(o) {
}

FileData::FileData(const char* data, unsigned int size) : DynamicBlob(size) {
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

