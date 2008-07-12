#include <roint/ro.h>

#include <iostream>
#include <fstream>
#include <string>


bool convert(const std::string& fn) {
	std::string prefix = "";
	unsigned int i;
	for (i = 0; i < fn.length(); i++) {
		if (fn[i] == '.')
			break;
		prefix += fn[i];
	}

	char ofn[256];
	std::cout << "Processing file " << fn << "..." << std::endl;
	std::fstream in(fn.c_str(), std::ios_base::in | std::ios_base::binary);
	if (!in.is_open()) {
		std::cerr << "Error: Cannot open file " << fn << std::endl;
		return(false);
	}

	RO::SPR spr;
	spr.readStream(in);
	std::cout << "\tFile has " << spr.getImgCount() << " frames." << std::endl;
	std::cout << "\tVersion " << (int)spr.getVersion()->cver.major << "." << (int)spr.getVersion()->cver.minor << std::endl;
	for (i = 0; i < spr.getImgCount(); i++) {
		sprintf(ofn, "%s-%d.bmp", prefix.c_str(), i);
		// std::cout << "\tExporting frame " << i << " to file " << ofn;
		const RO::SPR::Image* img = spr.getFrame(i);
		// std::cout << " (" << img->w << "x" << img->h << ")" << std::endl;
		std::ofstream out(ofn, std::ios_base::binary);
		if (!out.is_open())
			std::cerr << "Error opening file for write: " << ofn << std::endl;
		else {
			spr.saveBMP(i, out);
			out.close();
		}
	}

	in.close();
	return(true);
}

int main(int argc, char* argv[]) {
	std::cout << "Initializing OpenRagnarok SPR Exporter" << std::endl;

	int i;

	if (argc < 2) {
		std::cerr << "Invalid syntax." << std::endl
			<< "Usage: " << argv[0] << " filename1.spr [filename2.spr [filename3.spr [...]]]" << std::endl;
		return(1);
	}

	for (i = 1; i < argc; i++) {
		convert(argv[i]);
	}

	std::cout << "done." << std::endl;

	return 0;
}

