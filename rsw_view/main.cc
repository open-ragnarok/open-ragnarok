/* $Id$ */
#include "stdafx.h"

#define _SDL_main_h

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include "sdlengine.h"

#include "my_rsw.h"

void plane(int x = 5, int y = 5) {
	float fx, fy;
	fx = (float)x;
	fy = (float)y;

	glPushMatrix();
	glTranslatef(fx, 0, fy);
	glPopMatrix();
}

/** Oh, look! A cube! */
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

class RSWDraw : public SDLEngine {
protected:
	float rot_y;
	float z;
	float s;
	MyRsw rsw;
public:
	RSWDraw() : SDLEngine() {
		z = -5.0f;
		rot_y = 0.0f;
		s = 1.0f/64.0f;
	}

	RSWDraw(RO::GRF* grf) {
		rsw.setGRF(grf);
	}

	virtual ~RSWDraw() { }

	bool Read(const std::string& name) {
		return(rsw.Read(name));
	}

	bool Read(RO::GRF* grf, const std::string& name) {
		rsw.setGRF(grf);
		bool ret = rsw.Read(name);
		return(ret);
	}

	virtual void BeforeClose() {
		rsw.Clear();
	}

	virtual void Draw() {
		if (rot_y < 0) {
			// Weird... I guess something is leaking...
			rot_y = 0;
		}
		rot_y += 1.43f;
		printf("%.2f   \r", rot_y);
		glLoadIdentity();

		glTranslatef(0, -10, -25);
		//glTranslatef(0, 0, -25);
		glRotatef(rot_y, 0, 1, 0);
		// cube();
		rsw.Draw();
		//rsw.DrawModels();
		Sleep(3);
	}
};

int main(int argc, char* argv[]) {
	// === LOAD GRF ===
	std::string grf_fn;
	RO::GRF grf;
	grf_fn = "data/sdata.grf";
	grf.open(grf_fn);
	if (grf.isOpen()) {
		printf("GRF ok\n");
	}
	else {
		printf("can't find file '%s'\n", grf_fn.c_str());
		Sleep(5000);
		return(1);
	}

	std::string mapname = "prontera";
	if (argc > 1) {
		mapname = argv[1];
	}

	RSWDraw draw(&grf);
	// draw.InitDisplay(1280, 800, true);
	draw.z_near = 10.0f;
	draw.z_far = 1000.0f;
	draw.InitDisplay();
	if (!draw.Read(mapname)) {
		std::cerr << "Error reading map " << mapname << std::endl;
		draw.CloseDisplay();
		return(1);
	}
	int i;
	for (i = 0; i < 200; i++) {
		draw.Sync();
		Sleep(30);
	}
	draw.CloseDisplay();

	return(0); // keep compiler happy
}
