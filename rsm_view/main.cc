/* $Id$ */
#ifdef WIN32
	#define _CRT_SECURE_NO_WARNINGS
	#define _SDL_main_h
#endif

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include "sdlengine.h"

#include "ro.h"
#include "rogl.h"

#include "image_bmp.h"

class MyRSM {
protected:
	RO::RSM* rsm;

	unsigned int* textures;

public:
	MyRSM(const std::string& fn) {
	}

	unsigned int getTexture(unsigned int i) const {
		if (textures == NULL)
			return(0);
		if (rsm == NULL)
			return(0);
		if (i >= rsm->getTextureCount())
			return(0);
		return(textures[i]);
	}

	MyRSM() {
		rsm = NULL;
		grf = NULL;
		textures = NULL;
	}

	~MyRSM() {
		close();
	}

	void close() {
		if (textures != NULL) {
			if (rsm != NULL) {
				glDeleteTextures(rsm->getTextureCount(), textures);
				delete(rsm);
			}
			delete[] textures;
		}
		
		rsm = NULL;
		textures = NULL;
	}

	void Draw() const {
		ROGL::draw(rsm, textures);
	}

	RO::GRF* grf;

	bool read(const std::string& fn) {
		if (grf == NULL) {
			std::cerr << "Oops... No GRF file loaded" << std::endl;
			return(false);
		}
		std::string rsm_fn;
		rsm_fn = "data\\model\\";
		rsm_fn += fn;
		std::stringstream sdata;
		std::cout << rsm_fn << std::endl;
		if (!grf->write(rsm_fn, sdata)) {
			std::cerr << "Can't find file " << rsm_fn << std::endl;
			return(false);
		}
		rsm = new RO::RSM();
		if (!rsm->readStream(sdata)) {
			std::cerr << "Error parsing file " << rsm_fn << std::endl;
			delete(rsm);
			return(false);
		}

		loadTextures();

		return(true);
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

	void Dump() const {
		rsm->Dump(std::cout);
	}

	/**
	 * Grabs the texture file fn from the registered GRF and registers it in the GL texture texid.
	 * Texid must be already created.
	 * @param fn string filename of the texture
	 * @param texid integer texture id
	 */
	bool registerTexture(const std::string& fn, const int& texid) {
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
		/*
		char buf[256];
		sprintf(buf, "texture_%d.bmp", texid);
		grf->save(fn, buf);
		std::cout << "Textured saved to file " << buf << std::endl;
		*/
		return(true);
	}

	/** Loads all textures from the loaded rsm file */
	bool loadTextures() {
		unsigned int i;
		std::string texfn;
		if (textures != NULL)
			delete[] textures;
		if (rsm == NULL)
			return(false);
		textures = new unsigned int[rsm->getTextureCount()];
		glGenTextures(rsm->getTextureCount(), textures);

		glEnable(GL_TEXTURE_2D);
		for (i = 0; i < rsm->getTextureCount(); i++) {
			texfn = "data\\texture\\";
			texfn += rsm->getTexture(i);
			registerTexture(texfn, textures[i]);
		}
		glDisable(GL_TEXTURE_2D);

		return(true);
	}
};

void cube(float s = 1.0f) {
	float vertices[][3] = {
		{  s / 2,  s / 2,  s / 2 },
		{  s / 2,  s / 2, -s / 2 },
		{  s / 2, -s / 2,  s / 2 },
		{  s / 2, -s / 2, -s / 2 },
		{ -s / 2,  s / 2,  s / 2 },
		{ -s / 2,  s / 2, -s / 2 },
		{ -s / 2, -s / 2,  s / 2 },
		{ -s / 2, -s / 2, -s / 2 }
	};

	short indexes[] = {
		0, 1, 3, 2,
		0, 2, 6, 4,
		0, 1, 5, 4,
		1, 3, 7, 5,
		2, 3, 7, 6,
		4, 5, 7, 6
	};

#if 1

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_SHORT, indexes);

	glDisableClientState(GL_VERTEX_ARRAY);

#else
	glBegin(GL_QUADS);
	glVertex3f(vertices[0][0], vertices[0][1], vertices[0][2]);
	glVertex3f(vertices[1][0], vertices[1][1], vertices[1][2]);
	glVertex3f(vertices[3][0], vertices[3][1], vertices[3][2]);
	glVertex3f(vertices[2][0], vertices[2][1], vertices[2][2]);

	glVertex3f(vertices[0][0], vertices[0][1], vertices[0][2]);
	glVertex3f(vertices[2][0], vertices[2][1], vertices[2][2]);
	glVertex3f(vertices[6][0], vertices[6][1], vertices[6][2]);
	glVertex3f(vertices[4][0], vertices[4][1], vertices[4][2]);

	glVertex3f(vertices[0][0], vertices[0][1], vertices[0][2]);
	glVertex3f(vertices[1][0], vertices[1][1], vertices[1][2]);
	glVertex3f(vertices[5][0], vertices[5][1], vertices[5][2]);
	glVertex3f(vertices[4][0], vertices[4][1], vertices[4][2]);
	glEnd();
#endif
}

/**
 * Saves the grf file list into a stream, including a C unsigned char vector for copy-paste operation for testing purposes
 */
void saveList(const RO::GRF& grf, std::ostream& out) {
	unsigned int c = grf.getCount();
	unsigned int i, j;

	char buf[256];
	char aux[256];
	unsigned char cc;
	for (i = 0; i < c; i++) {
		strcpy(buf, grf.getFilename(i).c_str());
		out << buf << std::endl << "\t";
		out << "unsigned char fn[] = { ";
		for (j = 0; j < strlen(buf); j++) {
			if (j > 0)
				out << ", ";
			if ((buf[j] >= 'a' && buf[j] <= 'z') ||
				(buf[j] >= 'A' && buf[j] <= 'Z') ||
				(buf[j] >= '0' && buf[j] <= '9') ||
				buf[j] == '_') {
				out << "'" << buf[j] << "'";
			}
			else if (buf[j] == '\\') {
				out << "'\\\\'";
			}
			else {
				cc = (unsigned char)buf[j];
				sprintf(aux, "0x%x", cc);
				out << aux;
			}
		}
		out << ", 0 };" << std::endl;
	}
}

// We're using a specific file. This is encoded using EUC_KR
//unsigned char fn[] = { 0xc5, 0xa9, 0xb8, 0xae, 0xbd, 0xba,	0xb8, 0xb6, 0xbd, 0xba, 0xb8, 0xb6,	0xc0, 0xbb, '\\',  'x',  'm',  'a',	 's',  '_', 0xb5, 0xa5, 0xc4, 0xda,	0x36,  '.',  'r',  's',  'm', 0 };
//unsigned char fn[] = { 0xb3, 0xbb, 0xba, 0xce, 0xbc, 0xd2, 0xc7, 0xb0, '\\', 0xbf, 0xc0, 0xc5,	0xa9, 0xc0, 0xd4, 0xb1, 0xb8, 0xc4,	0xbf, 0xc5, 0xd9, 0x2e, 'r', 's', 'm', 0 };
//unsigned char fn[] = { 0xbe, 0xc6, 0xc0, 0xce, 0xba, 0xea, 0xb7, 0xce, 0xc5, 0xa9, '\\', 0xb9, 0xce, 0xb0, 0xa1, '0', '2', 0x2e, 'r', 's', 'm', 0 };
//unsigned char fn[] = { 0xc5, 0xa9, 0xb8, 0xae, 0xbd, 0xba, 0xb8, 0xb6, 0xbd, 0xba, 0xb8, 0xb6, 0xc0, 0xbb, '\\', 'x', 'm', 'a', 's', '_', 0xb5, 0xa5, 0xc4, 0xda, '8', 0x2e, 'r', 's', 'm' };
unsigned char fn[] = { 0xb6, 0xf3, 0xc7, 0xef, '\\', 0xb9, 0xce, 0xb0, 0xa1, '0', '3', 0x2e, 'r', 's', 'm', 0 };
char* model_fn = (char*)fn;

int main(int argc, char* argv[]) {
	std::string grf_fn;
	std::string rsm_fn;

	RO::GRF grf;

	grf_fn = "data/sdata.grf";
	grf.open(grf_fn);

	if (grf.isOpen()) {
		printf("ok\n");
	}
	else {
		printf("can't find file '%s'\n", grf_fn.c_str());
		return(1);
	}

#if 0
	std::ofstream of("grf_list.txt");
	saveList(grf, of);
	of.close();
#endif

	char *fnp = (char*)model_fn;

	SDLEngine engine;
	engine.InitDisplay();

	MyRSM rsm;
	rsm.grf = &grf;
	rsm.read(fnp);

	float z = -5.0f;
	float r = 0.0f;
	float s = 1.0f/64.0f;

	// rsm.Dump();
	engine.setTransparency(true);
	
	for (int i = 0; i < 1000; i++) {
		glLoadIdentity();
		glTranslatef(0, 0, z);
		glRotatef(r, 0, 1, 0);
		glPushMatrix();
		glScalef(s,s,s);
		rsm.Draw();
		//cube();
		glPopMatrix();
		engine.Sync();
		printf("\r%d", i);
		Sleep(9);
		// z+= 0.05f;
		r += 0.8f;
	}

	rsm.close();
	engine.CloseDisplay();
	grf.close();
	return(0);
}
