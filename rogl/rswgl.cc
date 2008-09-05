/* $Id$ */
#include "stdafx.h"

#include "rswgl.h"

bool ROGL::draw(const RO::GND* gnd, const unsigned int* textures) {
	if(textures != NULL) {
		printf("Yes! Textures!\n");
		glEnable(GL_TEXTURE_2D);
	}

	int w, h;
	int x, y;
	int last_texidx = -1;

	w = gnd->getWidth();
	h = gnd->getHeight();

	float vertsv[4][3];
	float texv[4][2];
	float color[4];

	glPushMatrix();
	glTranslatef(-(float)w / 2, 0.00f, -(float)h / 2);

	const RO::GND::strCube* cube;
	const RO::GND::strTile* tile;

	for (x = 0; x < w; x++) {
		for (y = 0; y < h; y++) {
			cube = &gnd->getCube(x, y);
			
#if 0
			vertsv[0][0] = (float)x;
			vertsv[0][2] = (float)y;
			vertsv[0][1] = 0.00f;

			vertsv[1][0] = (float)x + 1.00f;
			vertsv[1][2] = (float)y;
			vertsv[1][1] = 0.00f;

			vertsv[2][0] = (float)x + 1.00f;
			vertsv[2][2] = (float)y + 1.00f;
			vertsv[2][1] = 0.00f;

			vertsv[3][0] = (float)x;
			vertsv[3][2] = (float)y + 1.00f;
			vertsv[3][1] = 0.00f;

			glBegin(GL_QUADS);
			for (int k = 0; k < 4; k++) {
				//glVertex3fv(vertsv[k]);
				glVertex3f(vertsv[k][0], vertsv[k][1], vertsv[k][2]);
			}
			glEnd();
#endif

#if 1 /* TILE_UP */
			if (cube->tile_up > 0) {
				//TileUP
				tile = &gnd->getTile(cube->tile_up);
				
				if (textures != NULL) {
					int texidx = tile->texture_index;
					if (last_texidx != texidx) {
						last_texidx = texidx;
						glBindTexture(GL_TEXTURE_2D, texidx);
					}

					texv[0][0] = tile->texture_start[0];
					texv[0][1] = tile->texture_end[0];

					texv[1][0] = tile->texture_start[1];
					texv[1][1] = tile->texture_end[1];

					texv[2][0] = tile->texture_start[3];
					texv[2][1] = tile->texture_end[3];

					texv[3][0] = tile->texture_start[2];
					texv[3][1] = tile->texture_end[2];
				}

				vertsv[0][0] = (float)x;
				vertsv[0][2] = (float)y;
				vertsv[0][1] = -cube->height[0];

				vertsv[1][0] = (float)x + 1.00f;
				vertsv[1][2] = (float)y;
				vertsv[1][1] = -cube->height[1];

				vertsv[2][0] = (float)x + 1.00f;
				vertsv[2][2] = (float)y + 1.00f;
				vertsv[2][1] = -cube->height[3];

				vertsv[3][0] = (float)x;
				vertsv[3][2] = (float)y + 1.00f;
				vertsv[3][1] = -cube->height[2];

				glBegin(GL_QUADS);
				int k;
				for (k = 0; k < 4; k++)
					color[k] = (float)tile->color[k] / 255;
				// Note: To correctly mimic the official client, this should be glColor3fv()
				glColor4fv(color);
				for (k = 0; k < 4; k++) {
					if (textures != NULL)
						glTexCoord2fv(texv[k]);
					glVertex3fv(vertsv[k]);
				}
				glEnd();
			}
#endif
		}
	}

	glPopMatrix();
	if(textures != NULL)
		glDisable(GL_TEXTURE_2D);

	return(true);
}

bool ROGL::draw(const RO::RSW* rsw, const RO::GND* gnd, const unsigned int* textures) {
	if (rsw == NULL) {
#ifdef _DEBUG
		std::cerr << "ROGL::draw() ERROR: rsw pointer is null" << std::endl;
#endif
		return(false);
	}

	return(true);
}
