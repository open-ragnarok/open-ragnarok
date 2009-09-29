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
#include "stdafx.h"

#include "ronet/ucbuffer.h"

ronet::ucBuffer& ronet::ucBuffer::operator << (std::istream& s) {
	write(s);
	return(*this);
}

ronet::ucBuffer& ronet::ucBuffer::operator << (ucBuffer& b) {
	write(b);
	return(*this);
}

ronet::ucBuffer& ronet::ucBuffer::operator >> (std::ostream& s) {
	s.write((char*)data + dataStart, dataSize());
	alloc(0);
	return(*this);
}

ronet::ucBuffer& ronet::ucBuffer::operator >> (unsigned char& c) {
	if (dataSize() == 0)
		return(*this);
	c = *(data+dataStart);
	dataStart++;
	return(*this);
}

ronet::ucBuffer& ronet::ucBuffer::operator >> (short& c) {
	c = 0;
	if (dataSize() < sizeof(short))
		return(*this);

	c = *(short*)(data+dataStart);

	dataStart += sizeof(short);
	return(*this);
}

ronet::ucBuffer& ronet::ucBuffer::operator >> (unsigned short& c) {
	c = 0;
	if (dataSize() < sizeof(unsigned short))
		return(*this);

	c = *(unsigned short*)(data+dataStart);

	dataStart += sizeof(unsigned short);
	return(*this);
}

ronet::ucBuffer& ronet::ucBuffer::operator >> (int& c) {
	c = 0;
	if (dataSize() < sizeof(int))
		return(*this);

	c = *(int*)(data+dataStart);
	dataStart += sizeof(int);
	return(*this);
}

ronet::ucBuffer& ronet::ucBuffer::operator >> (unsigned int& c) {
	c = 0;
	if (dataSize() < sizeof(unsigned int))
		return(*this);

	c = *(unsigned int*)(data+dataStart);
	dataStart += sizeof(unsigned int);
	return(*this);
}

ronet::ucBuffer& ronet::ucBuffer::operator >> (char& c) {
	c = 0;
	if (dataSize() < sizeof(char))
		return(*this);

	c = *(unsigned int*)(data+dataStart);
	dataStart += sizeof(char);
	return(*this);
}
