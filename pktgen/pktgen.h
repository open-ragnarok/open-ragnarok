#ifndef __PKTGEN_H
#define __PKTGEN_H

#include "tinyxml/tinyxml.h"

#include <map>
#include <vector>
#include <stdio.h>

class PktGen {
protected:
	std::string pkt_file_prefix;
	std::string pktname;
	std::vector<std::string> params;
	std::map<std::string, std::string> param_type;
	std::map<std::string, int> param_size;

	int size;
	unsigned short pktid;

	/**
	 * Decodes a type into a valid C type.
	 * uint = unsigned int
	 * ushort = unsigned short
	 * uchar = unsigned char
	 */
	std::string decodeType(const std::string&);

	/**
	 * Retrieves the decoded type for a given parameter
	 */
	std::string getParameterType(const std::string&);

public:
	PktGen(TiXmlElement*);
	~PktGen();

	void OutputHeader(FILE*);
	void OutputSource(FILE*);
	void Output();
};

#endif /* __PKTGEN_H */