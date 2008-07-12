#include <roint/ro.h>

#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
	std::cout << "Initializing OpenRagnarok RSM Dumper" << std::endl;

	if (argc < 2) {
		std::cerr << "Invalid syntax." << std::endl
			<< "Usage: " << argv[0] << " filename1.rsm" << std::endl;
		return(1);
	}

	RO::RSM rsm;
	if (!rsm.read(argv[1])) {
		std::cout << "Error reading file " << argv[1] << std::endl;
		return(1);
	}
	rsm.Dump(std::cout);


	std::cout << "done." << std::endl;

	return 0;
}

