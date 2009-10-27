/* $Id$
 * blob.cc
 *
 *  Created on: 2009-09-09
 *      Author: Sergio Moura
 *    Revision: $Rev$
 */
#include "stdafx.h"
#include "sdle/blob.h"

namespace sdle {

DynamicBlob::DynamicBlob() : base_DynamicBlob<char>() {
}

DynamicBlob::DynamicBlob(const char* data, unsigned int size) : base_DynamicBlob<char>(size) {
	if (data != NULL)
		memcpy(buffer, data, dataSize);
	else {
		memset(buffer, 0, dataSize);
	}
}

DynamicBlob::DynamicBlob(unsigned int size) : base_DynamicBlob<char>(size) {
	memset(buffer, 0, dataSize);
}

DynamicBlob::DynamicBlob(const DynamicBlob& o) : base_DynamicBlob<char>(o.dataSize) {
	memcpy(buffer, o.buffer, dataSize);
}

DynamicBlob::~DynamicBlob() {

}

}
