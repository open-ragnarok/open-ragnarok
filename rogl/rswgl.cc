/* $Id$ */
#include "stdafx.h"

#include "rogl/rswgl.h"

#define L 10.0f

bool rogl::draw(const ro::GND* gnd, const unsigned int* textures) {
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

	float Rot[16];
	memset(Rot, 0, sizeof(float) * 16);
	Rot[0] = 1;
	Rot[6] = 1;
	Rot[9] = -1;
	Rot[15] = 1;
	glMultMatrixf(Rot);

	glScalef(L, L, 1.0f);

	glTranslatef(-(float)w / 2, -(float)h / 2, 0.00f);

	for (x = 0; x < w; x++) {
		for (y = 0; y < h; y++) {
			const ro::GND::Cell& cell = gnd->getCell(x, y);
			
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
			if (cell.topSurfaceId != -1) {
				const ro::GND::Surface& surface = gnd->getSurface(cell.topSurfaceId);
				
				if (textures != NULL) {
					int texidx = surface.textureId;
					if (last_texidx != texidx) {
						last_texidx = texidx;
						glBindTexture(GL_TEXTURE_2D, textures[texidx]);
					}

					texv[0][0] = surface.u[0];
					texv[0][1] = surface.v[0];

					texv[1][0] = surface.u[1];
					texv[1][1] = surface.v[1];

					texv[2][0] = surface.u[3];
					texv[2][1] = surface.v[3];

					texv[3][0] = surface.u[2];
					texv[3][1] = surface.v[2];
				}

				vertsv[0][0] = (float)x;
				vertsv[0][1] = (float)y;
				vertsv[0][2] = -cell.height[0];

				vertsv[1][0] = (float)x + 1.00f;
				vertsv[1][1] = (float)y;
				vertsv[1][2] = -cell.height[1];

				vertsv[2][0] = (float)x + 1.00f;
				vertsv[2][1] = (float)y + 1.00f;
				vertsv[2][2] = -cell.height[3];

				vertsv[3][0] = (float)x;
				vertsv[3][1] = (float)y + 1.00f;
				vertsv[3][2] = -cell.height[2];

				glBegin(GL_QUADS);
				int k;
				for (k = 0; k < 4; k++)
					color[k] = (float)surface.color[k] / 255;
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

#if 1 /* TILE_SIDE */
			if (cell.frontSurfaceId != -1) {
				const ro::GND::Cell& cell2 = gnd->getCell(x, y + 1);
				const ro::GND::Surface& surface = gnd->getSurface(cell.frontSurfaceId);
				
				if (textures != NULL) {
					int texidx = surface.textureId;
					if (last_texidx != texidx) {
						last_texidx = texidx;
						glBindTexture(GL_TEXTURE_2D, textures[texidx]);
					}

					texv[0][0] = surface.u[0];
					texv[0][1] = surface.v[0];

					texv[1][0] = surface.u[1];
					texv[1][1] = surface.v[1];

					texv[2][0] = surface.u[3];
					texv[2][1] = surface.v[3];

					texv[3][0] = surface.u[2];
					texv[3][1] = surface.v[2];
				}

				vertsv[0][0] = (float)x;
				vertsv[0][2] = (float)y + 1.00f;
				vertsv[0][1] = -cell.height[3];

				vertsv[1][0] = (float)x + 1.00f;
				vertsv[1][2] = (float)y + 1.00f;
				vertsv[1][1] = -cell.height[1];

				vertsv[2][0] = (float)x + 1.00f;
				vertsv[2][2] = (float)y + 1.00f;
				vertsv[2][1] = -cell2.height[0];

				vertsv[3][0] = (float)x;
				vertsv[3][2] = (float)y + 1.00f;
				vertsv[3][1] = -cell.height[2];

				glBegin(GL_QUADS);
				int k;
				for (k = 0; k < 4; k++)
					color[k] = (float)surface.color[k] / 255;
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

#if 1 /* TILE_ASIDE */
			if (cell.rightSurfaceId != -1) {
				const ro::GND::Cell& cell2 = gnd->getCell(x + 1, y);
				const ro::GND::Surface& surface = gnd->getSurface(cell.rightSurfaceId);
				
				if (textures != NULL) {
					int texidx = surface.textureId;
					if (last_texidx != texidx) {
						last_texidx = texidx;
						glBindTexture(GL_TEXTURE_2D, textures[texidx]);
					}

					texv[0][0] = surface.u[0];
					texv[0][1] = surface.v[0];

					texv[1][0] = surface.u[1];
					texv[1][1] = surface.v[1];

					texv[2][0] = surface.u[3];
					texv[2][1] = surface.v[3];

					texv[3][0] = surface.u[2];
					texv[3][1] = surface.v[2];
				}

				vertsv[0][0] = (float)x + 1.00f;
				vertsv[0][2] = (float)y + 1.00f;
				vertsv[0][1] = -cell.height[2];

				vertsv[1][0] = (float)x + 1.00f;
				vertsv[1][2] = (float)y;
				vertsv[1][1] = -cell.height[3];

				vertsv[2][0] = (float)x + 1.00f;
				vertsv[2][2] = (float)y;
				vertsv[2][1] = -cell2.height[1];

				vertsv[3][0] = (float)x + 1.00f;
				vertsv[3][2] = (float)y + 1.00f;
				vertsv[3][1] = -cell2.height[0];

				glBegin(GL_QUADS);
				int k;
				for (k = 0; k < 4; k++)
					color[k] = (float)surface.color[k] / 255;
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

bool rogl::draw(const ro::RSW* rsw, const ro::GND* gnd, const unsigned int* textures) {
	if (rsw == NULL) {
#ifdef _DEBUG
		std::cerr << "ROGL::draw() ERROR: rsw pointer is null" << std::endl;
#endif
		return(false);
	}

	return(true);
}
