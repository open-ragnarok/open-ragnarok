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
#ifndef __RO_OBJECT_H
#define __RO_OBJECT_H

#include <iostream>
#include <ostream>
#include <istream>
#include <string>

#include "roint_import.h"

namespace ro {

union s_obj_ver {
	short sver;
	struct _cver {
		unsigned char major;
		unsigned char minor;
	} cver;
};

class ROINT_DLLAPI Object {
public:
	Object();
	Object(const Object&);
	virtual ~Object();

	virtual bool read(const std::string&);
	virtual bool readStream(std::istream&) = 0;
	bool isValid() const;
	const s_obj_ver* getVersion() const;

protected:
	unsigned char magicSize;
	char magic[4];
	s_obj_ver m_version;
	bool m_valid;

	/**
	 * Checks compatibility with a given version.
	 * Returns true if the current version is greater or equal the one given
	 */
	bool IsCompatibleWith(unsigned char major_ver, unsigned char minor_ver) const;

	/**
	 * Checks compatibility with a given version.
	 * Returns true if the current version is greater or equal the one given
	 */
	bool IsCompatibleWith(short ver) const;

	/**
	 * Reads the header from the input stream.
	 * This has variable size, being 2 bytes for the version and magicSize bytes (usually 4) for the fingerprint.
	 */
	bool readHeader(std::istream&);

	/**
	 * Writes the header to the output stream
	 */
	bool writeHeader(std::ostream&) const;

	/**
	 * Copies the header to another object.
	 */
	bool copyHeader(Object*) const;

	/**
	 * Copies the header to another object.
	 */
	bool copyHeader(Object&) const;

	/**
	 * Checks if we have a valid header according to the string given.
	 */
	bool checkHeader(const std::string&) const;

public:
#ifdef ROINT_USE_XML
	/**
	 * Generates the XML root object. This should be overloaded by child classes (but it's not mandatory).
	 */
	virtual TiXmlElement *GenerateXML(const std::string& name = "", bool utf = true) const;

	/**
	 * Generates the XML document from the GenerateXML function
	 */
	TiXmlDocument GenerateXMLDoc(const std::string& name = "", bool utf = true) const;

	/**
	 * Saves the XML to an output stream
	 */
	bool SaveXML(std::ostream& out, const std::string& name = "", bool utf = true) const;

	/**
	 * Saves the XML to a file
	 */
	bool SaveXML(const std::string& fn, const std::string& name = "", bool utf = true) const;
#endif
};

} /* namespace ro */

#endif /* __RO_OBJECT_H */
