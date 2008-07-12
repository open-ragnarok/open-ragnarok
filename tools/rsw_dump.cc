#include <roint/ro.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

void printSyntax(const char* me) {
	std::cout
		<< "Usage: " << me << " [options] filename.rsw [filename.rsw ...]" << std::endl
		<< "Options are:" << std::endl
		<< "\t--grf <filename.grf>\tReads from specified grf file" << std::endl;
}

void DumpRSW(const std::string& fn) {
	RO::RSW rsw;
	if (!rsw.read(fn)) {
		std::cout << "Error reading file " << fn << std::endl;
		return;
	}
	std::stringstream ss;
	rsw.Dump(ss);
	RO::euc2utf8(ss, std::cout);
}

void DumpRSWFromGRF(const std::string& fn, RO::GRF& grf) {
	RO::RSW rsw;
	std::stringstream fd;
	std::string file = "data\\";
	file += fn;	
	if (!grf.write(file, fd)) {
		std::cerr << "Can't read file " << fn << " from the specified GRF file" << std::endl;
		return;
	}
	if (!rsw.readStream(fd)) {
		std::cout << "Error reading file " << fn << std::endl;
		return;
	}
	std::stringstream ss;
	rsw.Dump(ss);
	RO::euc2utf8(ss, std::cout);
}

int main(int argc, char* argv[]) {
	std::cout << "Initializing OpenRagnarok RSW Dumper" << std::endl;
	
	std::string fn = "";
	std::string grf_fn = "";

	if (argc < 2) {
		std::cerr << "Invalid syntax." << std::endl;
		printSyntax(argv[0]);
		return(1);
	}

	bool found = false;

	RO::GRF grf;
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "--grf")) {
			i++;
			if (i == argc) {
				std::cerr << "Invalid syntax." << std::endl;
				printSyntax(argv[0]);
				return(1);
			}
			grf_fn = argv[i];
			if (grf.isOpen())
				grf.close();
			if (!grf.open(grf_fn)) {
				std::cerr << "Error opening grf file" << std::endl;
				return(1);
			}
			std::cout << "GRF opened" << std::endl;
		}
		else {
			found = true;
			if (!grf.isOpen()) {
				DumpRSW(argv[i]);
			}
			else {
				DumpRSWFromGRF(argv[i], grf);
			}
		}
	}

	if (found == false) {
		std::cerr << "Invalid syntax." << std::endl;
		printSyntax(argv[0]);
		return(1);
	}

	std::cout << "done." << std::endl;

	return 0;
}

