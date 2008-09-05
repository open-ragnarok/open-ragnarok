/* $Id$ */
#ifndef __MY_RSW_H
#define __MY_RSW_H

#include "ro.h"
#include "rogl.h"

#include "image_bmp.h"

class MyRsw {
protected:
	RO::RSW *rsw;
	RO::GND *gnd;

	/** The RO::GRF object used to read the files from. This pointer is not managed by this class */
	RO::GRF *grf;
	unsigned int gl_list;
	unsigned int *textures;
public:
	MyRsw() {
		rsw = NULL;
		textures = NULL;
		grf = NULL;
		gnd = NULL;
		gl_list = 0;
	}

	MyRsw(RO::GRF* grf) {
		rsw = NULL;
		gnd = NULL;
		textures = NULL;
		this->grf = grf;
	}

	void setGRF(RO::GRF* grf) {
		this->grf = grf;
	}

	MyRsw(RO::GRF* grf, const std::string& fn) {
		rsw = NULL;
		textures = NULL;
		gnd = NULL;
		this->grf = grf;
		Read(fn);
	}

	/**
	 * Reads the sdata stream as an bitmap file and registers it into texid
	 */
	bool registerTexture(std::istream& sdata, const int& texid) {
		ImageBMP bmp(sdata);

		glBindTexture(GL_TEXTURE_2D, texid);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // <<- Life saver!!!

		/*
		http://www.opengl.org/sdk/docs/man/xhtml/glTexImage2D.xml
		GL_INVALID_VALUE is generated if non-power-of-two textures are not supported and the width or height cannot be represented as 
		2^k + 2*border for some integer value of k.
		*/

		// Sanity check
		if (bmp.getWidth() <= 0 || bmp.getHeight() <= 0 || bmp.getBpp() <= 8 || bmp.getBuffer() == NULL) {
			std::cerr << "Invalid bitmap" << std::endl;
			bmp.Dump(std::cout);
			return(false);
		}

		int drawtype = GL_RGB;
		if (bmp.getBpp() == 32) {
			drawtype = GL_RGBA;
		}

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			bmp.getWidth(),
			bmp.getHeight(),
			0,
			drawtype,
			GL_UNSIGNED_BYTE,
			bmp.getBuffer()
		);

		return(true);
	}

	/**
	 * Grabs the texture file fn from the registered GRF and registers it in the GL texture texid.
	 * Texid must be already created.
	 * @param fn string filename of the texture
	 * @param texid integer texture id
	 * @param out string filename to save the texture to. If empty string, it will not be saved.
	 */
	bool registerTexture(const std::string& fn, const int& texid, const std::string& out = "") {
		if (grf == NULL)
			return(false);
		std::stringstream sdata;
		if (!grf->write(fn, sdata)) {
			std::cerr << "Error loading texture (" << texid << ") " << fn << std::endl;
			return(false);
		}
		if (!registerTexture(sdata, texid)) {
			std::cerr << "Erorr registering texture (" << texid << ") " << fn << std::endl;
			return(false);
		}
		// Save texture
		if (out != "") {
			grf->save(fn, out.c_str());
			std::cout << "Texture saved to file " << out << std::endl;
		}
		return(true);
	}

	bool LoadTextures() {
		// Sanity Check
		if (grf == NULL) {
			std::cerr << "MyRSW::Read() ERROR: No GRF to read the textures from" << std::endl;
			return(false);
		}
		if (gnd == NULL) {
			std::cerr << "MyRSW::Read() ERROR: No GND to read the texture names" << std::endl;
			return(false);
		}
		int i;
		int count = gnd->getTextureCount();

		textures = new unsigned int[count];
		glGenTextures(count, textures);
		char buf[256];
		char buf2[256];

		glEnable(GL_TEXTURE_2D);
		for (i = 0; i < count; i++) {
			sprintf(buf, "data\\texture\\%s", gnd->getTexture(i).path);
			sprintf(buf2, "texture_%d.bmp", i);
			registerTexture(buf, textures[i], buf2);
			printf(">> %d: %s\n", textures[i], buf);
		}
		glDisable(GL_TEXTURE_2D);

		return(true);
	}

	bool Read(const std::string& name) {
		char fn[256];
		if (grf == NULL) {
			std::cerr << "MyRSW::Read() ERROR: No GRF set" << std::endl;
			return(false);
		}
		Clear();
		std::stringstream srsw, sgnd;
		// === RSW ===
		sprintf(fn, "data\\%s.rsw", name.c_str());
		if (!grf->write(fn, srsw)) {
			std::cerr << "MyRSW::Read() ERROR: Could not find file " << fn << " in the given GRF" << std::endl;
			Clear();
			return(false);
		}

		rsw = new RO::RSW();
		if (!rsw->readStream(srsw)) {
			std::cerr << "MyRSW::Read() ERROR: Error parsing file " << fn << std::endl;
			Clear();
			return(false);
		}

		// === GND ===
		sprintf(fn, "data\\%s", rsw->gnd_file);
		if (!grf->write(fn, sgnd)) {
			std::cerr << "MyRSW::Read() ERROR: Could not find file " << fn << " in the given GRF" << std::endl;
			Clear();
			return(false);
		}
		gnd = new RO::GND();
		if (!gnd->readStream(sgnd)) {
			std::cerr << "MyRSW::Read() ERROR: Error parsing file " << fn << std::endl;
			Clear();
			return(false);
		}

#ifdef _DEBUG
		// rsw->Dump(std::cout);
		gnd->Dump(std::cout);
#endif

		LoadTextures();
		return(true);
	}

	void Clear() {
		if (rsw != NULL)
			delete(rsw);
		if (gnd != NULL)
			delete(gnd);
		if (textures != NULL)
			delete[] textures;
		if (glIsList(gl_list))
			glDeleteLists(gl_list, 1);
		
		gl_list = 0;
		rsw = NULL;
		gnd = NULL;
		textures = NULL;
	}

	void Draw() {
		if (glIsList(gl_list)) {
			glCallList(gl_list);
			return;
		}
		gl_list = glGenLists(1);
		glNewList(gl_list, GL_COMPILE_AND_EXECUTE);
		printf("Generating list\n");
		ROGL::draw(gnd, textures);
		// ROGL::draw(gnd);
		glEndList();
	}
};

#endif /* __MY_RSW_H */
