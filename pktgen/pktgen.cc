#include "stdafx.h"

#include "pktgen.h"

static const char* fileHeader =
"/* The Open-Ragnarok Project             *\n"
" * http://www.open-ragnarok.org          *\n"
" * THIS FILE IS GENERATED AUTOMATICALLY. *\n"
" * DO NOT EDIT IT.                       *\n"
" */\n";

PktGen::PktGen(TiXmlElement* node) {
	std::string nodetype = node->Value();
	if (nodetype != "packet") {
		fprintf(stderr, "Invalid node type: %s\n", nodetype.c_str());
		return;
	}

	size = 0;
	pktid = 0;


	const TiXmlAttribute* attr = node->FirstAttribute();
	std::string attrname;

	while (attr != NULL) {
		attrname = attr->Name();
		if (attrname == "name") {
			pktname = attr->Value();
		}
		if (attrname == "id") {
			pktid = attr->IntValue();
		}
		if (attrname == "size") {
			size = attr->IntValue();
		}
		attr = attr->Next();
	}

	if (pktname == "") {
		fprintf(stderr, "No packet name defined.\n");
		return;
	}

	printf("Parsing packet %s...\n", pktname.c_str());

	TiXmlElement *parameters;

	parameters = node->FirstChildElement();
	std::string paramtype;
	int attrsize;
	while (parameters != NULL) {
		paramtype = "uint";
		attrsize = 0;

		attr = parameters->FirstAttribute();
		while (attr != NULL) {
			attrname = attr->Name();
			if (attrname == "type") {
				paramtype = attr->Value();
			}
			if (attrname == "size") {
				attrsize = attr->IntValue();
			}
			attr = attr->Next();
		}

		params.push_back(parameters->Value());
		param_type[parameters->Value()] = paramtype;
		param_size[parameters->Value()] = attrsize;
		parameters = parameters->NextSiblingElement();
	}

	pkt_file_prefix = "pkt_";
	pkt_file_prefix += pktname;

	Output();
}

std::string PktGen::decodeType(const std::string& v) {
	std::string ptype;

	if (v == "uint") {
		return("unsigned int");
	}
	if (v == "uchar") {
		return("unsigned char");
	}
	if (v == "ushort") {
		return("unsigned short");
	}

	return(v);
}

std::string PktGen::getParameterType(const std::string& p) {
	return(decodeType(param_type[p]));
}

void PktGen::Output() {
	FILE* out;
	char fname[128];

	sprintf(fname, "%s.h", pkt_file_prefix.c_str());
	out = fopen(fname, "w");
	OutputHeader(out);
	fclose(out);

	sprintf(fname, "%s.cc", pkt_file_prefix.c_str());
	out = fopen(fname, "w");
	OutputSource(out);
	fclose(out);
}

void PktGen::OutputHeader(FILE* out) {
	std::vector<std::string>::const_iterator itr;
	fprintf(out, "%s", fileHeader);

	char headerdef[512];
	sprintf(headerdef, "___AUTOGEN_PACKET_%s_H_", pktname.c_str());

	fprintf(out, "#ifndef %s\n#define %s\n\n", headerdef, headerdef);

	fprintf(out, "#include \"ronet/packet.h\"\n\n");
	fprintf(out, "namespace ronet {\n");
	fprintf(out, "class RONET_DLLAPI pkt%s : public Packet {\n", pktname.c_str());
	fprintf(out, "protected:\n");

	// Parameters
	itr = params.begin();
	std::string ptype;
	while (itr != params.end()) {
		ptype = getParameterType(*itr);

		// Check if we are an array
		if (param_size[*itr] > 0) {
			fprintf(out, "\t%s %s[%d];\n", ptype.c_str(), (*itr).c_str(), param_size[*itr]);
		}
		else {
			fprintf(out, "\t%s %s;\n", ptype.c_str(), (*itr).c_str());
		}
		itr++;
	}

	fprintf(out, "\n\tvirtual bool PrepareData();\n");
	fprintf(out, "public:\n");
	fprintf(out, "\tpkt%s();\n", pktname.c_str());
	fprintf(out, "\t~pkt%s();\n", pktname.c_str());
	fprintf(out, "\t\n");
	fprintf(out, "\tvirtual bool Decode(ucBuffer&);\n");
	fprintf(out, "\t\n");

	// Getters and setters declaration
	itr = params.begin();
	while (itr != params.end()) {
		ptype = getParameterType(*itr);

		fprintf(out, "\tconst %s& get%s() const;\n", ptype.c_str(), (*itr).c_str());
		if (param_size[*itr] > 0) {
			fprintf(out, "\tvoid set%s(const %s*) const;\n", (*itr).c_str(), ptype.c_str());
		}
		else {
			fprintf(out, "\tvoid set%s(const %s&) const;\n", (*itr).c_str(), ptype.c_str());
		}
		fprintf(out, "\t\n");
		itr++;
	}
	fprintf(out, "};\n");
	fprintf(out, "} /* namespace ronet */\n\n");

	fprintf(out, "#endif /* %s */\n", headerdef);
}

void PktGen::OutputSource(FILE* out) {
	char ID[32];
	std::vector<std::string>::const_iterator itr;
	std::string ptype;
	if (pktid > 0) {
		sprintf(ID, "0x%04d", pktid);
	}
	else {
		sprintf(ID, "pkt%sID", pktname.c_str());
	}

	fprintf(out, "%s", fileHeader);
	fprintf(out, "#include \"stdafx.h\"\n");
	fprintf(out, "#include \"ronet/packets/%s.h\"\n", pkt_file_prefix.c_str());

	fprintf(out, "namespace ronet {\n");
	fprintf(out, "\n");
	fprintf(out, "pkt%s::pkt%s() : Packet(%s) {\n", pktname.c_str(), pktname.c_str(), ID);
	if (size > 0) {
		fprintf(out, "\tsetSize(%d);\n", size);
	}
	fprintf(out, "}\n");
	fprintf(out, "\n");
	fprintf(out, "pkt%s::~pkt%s() {}\n", pktname.c_str(), pktname.c_str(), ID);
	fprintf(out, "\n");

	// Prepare data
	fprintf(out, "bool pkt%s::PrepareData() {\n", pktname.c_str());
	fprintf(out, "\tunsigned char* ptr = buffer;\n");
	itr = params.begin();
	while (itr != params.end()) {
		ptype = getParameterType(*itr);

		// Check if we are an array
		if (param_size[*itr] > 0) {
			fprintf(out, "\tmemcpy(ptr, %s, sizeof(%s) * %d);\n", (*itr).c_str(), ptype.c_str(), param_size[*itr]);
			fprintf(out, "\tptr += sizeof(%s) * %d;\n", ptype.c_str(), param_size[*itr]);
		}
		else {
			fprintf(out, "\tmemcpy(ptr, (unsigned char*)&%s, sizeof(%s));\n", (*itr).c_str(), ptype.c_str());
			fprintf(out, "\tptr += sizeof(%s);\n", ptype.c_str());
		}
		itr++;
	}
	fprintf(out, "\treturn(true);\n");
	fprintf(out, "}\n");

	// Decode data
	fprintf(out, "bool pkt%s::Decode(ucBuffer& buf) {\n", pktname.c_str());
	fprintf(out, "\tunsigned short buf_id;\n");
	fprintf(out, "\tunsigned short size;\n");
	fprintf(out, "\tbuf.peek((unsigned char*)&buf_id, 2);\n");
	fprintf(out, "\tif (buf_id != id) {\n");
	fprintf(out, "\t\tfprintf(stderr, \"Wrong packet id! (%04x != %04x)\\n\", id, buf_id);\n");
	fprintf(out, "\t\treturn(false);\n");
	fprintf(out, "\t}\n");
	fprintf(out, "\tsize = *(unsigned short*)(buf.getBuffer() + 2);\n");

	fprintf(out, "\tif (buf.dataSize() < %d) // Not enough data\n", size);
	fprintf(out, "\t\treturn(false);\n");
	fprintf(out, "\tbuf.ignore(2);\n");

	itr = params.begin();
	while (itr != params.end()) {
		ptype = getParameterType(*itr);

		// Check if we are an array
		if (param_size[*itr] > 0) {
			fprintf(out, "\tbuf.read((unsigned char*)%s, sizeof(%s) * %d);\n", (*itr).c_str(), ptype.c_str(), param_size[*itr]);
		}
		else {
			fprintf(out, "\tbuf >> %s;\n", (*itr).c_str());
		}
		itr++;
	}
	fprintf(out, "\treturn(true);\n");
	fprintf(out, "}\n\n");

	// Getters and Setters
	itr = params.begin();
	while (itr != params.end()) {
		ptype = getParameterType(*itr);

		fprintf(out, "const %s& pkt%s::get%s() const {\n", ptype.c_str(), pktname.c_str(), (*itr).c_str());
		fprintf(out, "\treturn((const %s&)%s);\n}\n", ptype.c_str(), (*itr).c_str());

		//TODO: Fix this (below)
		/*if (param_size[*itr] > 0) {
			fprintf(out, "\nvoid pkt%s::set%s(const %s*) const {\n", pktname.c_str(), (*itr).c_str(), ptype.c_str());
		}
		else {
			fprintf(out, "\nvoid pkt%s::set%s(const %s&) const {\n", pktname.c_str(), (*itr).c_str(), ptype.c_str());
		}*/

		fprintf(out, "\t\n");
		itr++;
	}


	fprintf(out, "\n} /* namespace ronet */\n");
}

PktGen::~PktGen() {
}