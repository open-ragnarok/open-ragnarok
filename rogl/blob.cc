#include "stdafx.h"

#include "rogl/blob.h"

unsigned char& rogl::DynamicBlob::operator[](int index) {
	unsigned long d = index;
	if (d >= dataSize) {
		fprintf(stderr, "Bad index on Blob::[]");
		return(buffer[0]);
	}
	return (buffer[index]);
}

const unsigned char& rogl::DynamicBlob::operator[](int index) const {
	unsigned long d = index;
	if (d >= dataSize) {
		fprintf(stderr, "Bad index on Blob::[]");
		return(buffer[0]);
	}
	return (buffer[index]);
}

unsigned char* rogl::DynamicBlob::operator *() {
	return (buffer);
}

rogl::DynamicBlob::DynamicBlob(const unsigned long& size) {
	dataSize = size;
	buffer = 0;
	if (size) {
		buffer = new unsigned char[size];
		assert(buffer!=0 && "DynamicBlob buffer could not be created - out of memory");
	}
}

rogl::DynamicBlob::DynamicBlob(const DynamicBlob& o) {
	buffer = NULL;
	setSize(o.dataSize);
	if (dataSize > 0)
		memcpy(buffer, o.buffer, dataSize);
}

void rogl::DynamicBlob::setSize(const unsigned long& size) {
	if (size == dataSize)
		return;
	unsigned char* newdata = NULL;
	if (size) {
		newdata = new unsigned char[size];
		assert(newdata !=0 && "DynamicBlob buffer could not be created - out of memory");
	}
	if (buffer) {
		if (size > 0)
			memcpy(newdata, buffer, (dataSize>size)?size:dataSize);
		delete[] buffer;
	}
	dataSize = size;
	buffer = newdata;
}

unsigned char& rogl::DynamicBlob::get(unsigned long index) {
	if (index >= dataSize) {
		fprintf(stderr, "Bad index on Blob::[]");
		return(buffer[0]);
	}
	return(buffer[index]);
}

const unsigned char& rogl::DynamicBlob::get(unsigned long index) const {
	if (index >= dataSize) {
		fprintf(stderr, "Bad index on Blob::[]");
		return(buffer[0]);
	}
	return(buffer[index]);
}
	
rogl::DynamicBlob::~DynamicBlob() {
	if (buffer)
		delete[] buffer;
	buffer = 0;
}

unsigned long rogl::DynamicBlob::size() const {
	return (dataSize * sizeof(unsigned char) + sizeof(this));
}

unsigned long rogl::DynamicBlob::blobSize() const {
	return(dataSize);
}

const unsigned char* rogl::DynamicBlob::getBuffer() const {
	return(buffer);
}

unsigned char* rogl::DynamicBlob::getBuffer() {
	return(buffer);
}

void rogl::DynamicBlob::clear() {
	if (buffer)
		delete[] buffer;
	buffer = NULL;
	dataSize = 0;
}

rogl::DynamicBlob& rogl::DynamicBlob::operator = (const rogl::DynamicBlob& o) {
	clear();
	this->setSize(o.dataSize);
	if (dataSize > 0)
		memcpy(buffer, o.buffer, dataSize);
	return(*this);
}

void rogl::DynamicBlob::write(std::ostream& o) const {
	o.write((char*)buffer, dataSize);
}

rogl::DynamicBlobChar::DynamicBlobChar(const unsigned long& size) : DynamicBlob(size) {}
rogl::DynamicBlobChar::DynamicBlobChar(const DynamicBlobChar& o) : DynamicBlob((DynamicBlob&)o) {}
rogl::DynamicBlobChar::DynamicBlobChar(const DynamicBlob& o) : DynamicBlob((DynamicBlob&)o) {}


rogl::DynamicBlobChar& rogl::DynamicBlobChar::operator = (const DynamicBlobChar& o) {
	clear();
	this->setSize(o.dataSize);
	if (dataSize > 0)
		memcpy(buffer, o.buffer, dataSize);
	return(*this);
}

char& rogl::DynamicBlobChar::operator[](int index) {
	unsigned long d = index;
	if (d >= dataSize) {
		fprintf(stderr, "Bad index on Blob::[]");
		return((char&)buffer[0]);
	}
	return ((char&)buffer[index]);
}

const char& rogl::DynamicBlobChar::operator[](int index) const {
	unsigned long d = index;
	if (d >= dataSize) {
		fprintf(stderr, "Bad index on Blob::[]");
		return((char&)buffer[0]);
	}
	return ((char&)buffer[index]);
}

char* rogl::DynamicBlobChar::operator*() {
	return((char*)buffer);
}
