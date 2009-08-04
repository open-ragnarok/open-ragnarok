#include "stdafx.h"

#include <stdio.h>

#include "pktgen.h"

int main(int argc, char* argv[]) {
	printf("OpenRO Packet Generator\n");
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <packet_descriptor>.xml\n", argv[0]);
		return(1);
	}

	printf("Decoding file %s...\n", argv[1]);

	TiXmlDocument doc;
	doc.LoadFile(argv[1]);

	TiXmlElement* node = doc.FirstChildElement();
	std::string nodetype = node->Value();
	if (nodetype == "packets") {
		TiXmlElement* parent = node;
		node = parent->FirstChildElement();
		while (node != NULL) {
			PktGen x(node);
			node = node->NextSiblingElement();
		}
	}
	else {
		PktGen x(node);
	}

	return(0);
}
