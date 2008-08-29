#include <stdio.h>
#include <iostream>
#include <sstream>

#define _SDL_main_h
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

	bool registerTexture(std::istream& sdata, int texid) {
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

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			bmp.getBpp() / 8,
			bmp.getWidth(),
			bmp.getHeight(),
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			bmp.getBuffer()
		);

		return(true);
	}

	void Dump() const {
		rsm->Dump(std::cout);
	}

	bool loadTextures() {
		unsigned int i;
		std::stringstream sdata;
		std::string texfn;
		if (textures != NULL)
			delete[] textures;
		textures = new unsigned int[rsm->getTextureCount()];
		glGenTextures(rsm->getTextureCount(), textures);

		for (i = 0; i < rsm->getTextureCount(); i++) {
			texfn = "data\\texture\\";
			texfn += rsm->getTexture(i);
			if (!grf->write(texfn, sdata)) {
				std::cerr << "Error loading texture (" << i << ") " << texfn << std::endl;
			}
			
			registerTexture(sdata, textures[i]);
			sdata.clear();
		}
		return(true);
	}

};

unsigned char model_fn[] = {
	0xc5, 0xa9, 0xb8, 0xae, 0xbd, 0xba,
	0xb8, 0xb6, 0xbd, 0xba, 0xb8, 0xb6,
	0xc0, 0xbb, '\\',  'x',  'm',  'a',
	 's',  '_', 0xb5, 0xa5, 0xc4, 0xda,
	0x36,  '.',  'r',  's',  'm', 0
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

int main(int argc, char* argv[]) {
	std::string grf_fn;
	std::string rsm_fn;

	RO::GRF grf;

	grf_fn = "../data/sdata.grf";
	grf.open(grf_fn);

	if (grf.isOpen()) {
		printf("ok\n");
	}
	else {
		printf("can't find file '%s'\n", grf_fn.c_str());
		return(1);
	}

	char *fnp = (char*)model_fn;

	MyRSM rsm;
	rsm.grf = &grf;
	rsm.read(fnp);

	SDLEngine engine;
	engine.InitDisplay();

	float z = -5.0f;
	float r = 0.0f;

	for (int i = 0; i < 500; i++) {
		glLoadIdentity();
		glTranslatef(0, 0, z);
		glRotatef(r, 0, 1, 0);
		//rsm.Draw();
		cube();
		engine.Sync();
		printf("\r%d", i);
		Sleep(90);
		// z+= 0.05f;
		r += 0.8f;
	}

	engine.CloseDisplay();
	rsm.close();
	grf.close();
	return(0);
}
