#define _CRT_SECURE_NO_WARNINGS // Disable secure warnings
#include <windows.h>
#pragma comment(lib, "sdl.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "freetype.lib")
#ifdef TEST_DLL
#	pragma comment(lib, "zlib.dll.lib")
#	pragma comment(lib, "libpng.dll.lib")
#	pragma comment(lib, "sdlengine.dll.lib")
#else
#	pragma comment(lib, "zlib.lib")
#	pragma comment(lib, "libpng.lib")
#	pragma comment(lib, "sdlengine.lib")
#endif

#include "sdle/sdl_engine.h"
#include "sdle/texture_png.h"
#include "sdle/font.h"

void cube(const float& side) {
	float aux = side / 2;
	// Thanks, NeHe! http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=06
	glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-aux, -aux,  aux);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f( aux, -aux,  aux);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( aux,  aux,  aux);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-aux,  aux,  aux);	// Top Left Of The Texture and Quad
		// Back Face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-aux, -aux, -aux);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-aux,  aux, -aux);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( aux,  aux, -aux);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( aux, -aux, -aux);	// Bottom Left Of The Texture and Quad
		// Top Face
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-aux,  aux, -aux);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-aux,  aux,  aux);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f( aux,  aux,  aux);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( aux,  aux, -aux);	// Top Right Of The Texture and Quad
		// Bottom Face
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-aux, -aux, -aux);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( aux, -aux, -aux);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( aux, -aux,  aux);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-aux, -aux,  aux);	// Bottom Right Of The Texture and Quad
		// Right face
		glTexCoord2f(1.0f, 0.0f); glVertex3f( aux, -aux, -aux);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( aux,  aux, -aux);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( aux,  aux,  aux);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( aux, -aux,  aux);	// Bottom Left Of The Texture and Quad
		// Left Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-aux, -aux, -aux);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-aux, -aux,  aux);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-aux,  aux,  aux);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-aux,  aux, -aux);	// Top Left Of The Texture and Quad
	glEnd();
}

int main(int argc, char* argv[]) {
	sdle::SDLEngine e;

	e.InitDisplay();

	const bool* keys = e.getKeys();
	glEnable(GL_TEXTURE_2D);

	// Load a texture
	sdle::Texture t;
	t = sdle::loadPngTexture("a-1.png");
	if (!t.Valid()) {
		printf("Texture not valid!\n");
		return(1);
	}

	// Load a font
	sdle::Font f;
	f.open("verdana.ttf", 12);
	if (!f.valid()) {
		printf("font not valid!\n");
		return(2);
	}

	e.setNearClip(1.0f);
	e.setFarClip(20.0f);
	float r = 0.0f;
	while (!keys[SDLK_ESCAPE]) {
		e.LookAt(8, 4, 0);
		glRotatef(r, 0, 1, 0);
		t.Activate();
		cube(1.0f);

		e.Mode2DStart();
		f.drawText(5, 5, "Drawing text using a ttf font");
		f.drawText(5, 200, "Press ESCAPE to exit.");

		e.Sync(30);
		e.ProcessKeyboard();
		r += 0.5f;
	}

	return(0); // Keep compiler happy
}