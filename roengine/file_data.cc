/* $Id$ */
#include "stdafx.h"

#include "file_data.h"

FileData::FileData() :
	DynamicBlob(),
	std::basic_iostream<char>(this),
	std::basic_streambuf<char>()  {

}

FileData::FileData(const FileData& o) : DynamicBlob(o), std::basic_iostream<char>(this), std::basic_streambuf<char>() {
}

FileData::FileData(const char* data, unsigned int size) : DynamicBlob(size), std::basic_iostream<char>(this), std::basic_streambuf<char>() {
	memcpy(this->buffer, data, size);
}

bool FileData::write(std::ostream& os) const {
	os.write(this->buffer, dataSize);
	return(true);
}

FileData& FileData::operator = (const FileData& o) {
	setSize(o.dataSize);
	memcpy(buffer, o.buffer, dataSize);

	return(*this);
}

