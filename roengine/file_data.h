/* $Id$ */
#ifndef __FILE_DATA_H
#define __FILE_DATA_H

#include "blob.h"

#include <iostream>

class FileData :
	public DynamicBlob<char>,
	public std::basic_streambuf<char>,
	public std::basic_iostream<char>
		{
public:
	FileData();
	FileData(const FileData& o);
	FileData(const char* data, unsigned int size);
	bool write(std::ostream& os) const;

	FileData& operator = (const FileData&);
};

#endif /* __FILE_DATA_H */