/* $Id$ */
#ifndef __FILE_DATA_H
#define __FILE_DATA_H

#include "rogl/blob.h"

#include <iostream>

class FileData : public rogl::DynamicBlobChar {
public:
	FileData();
	FileData(const FileData& o);
	FileData(const char* data, unsigned int size);
	bool write(std::ostream& os) const;

	FileData& operator = (const FileData&);
};

#endif /* __FILE_DATA_H */
