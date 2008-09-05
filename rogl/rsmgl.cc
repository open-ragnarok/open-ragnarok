/* $Id$ */
#include "stdafx.h"

#include "rsmgl.h"

bool ROGL::draw(const RO::RSM* rsm, const unsigned int* textures, int time) {
	if (rsm == NULL)
		return(false);

	const RO::RSM::Surface* surface;
	const RO::RSM::Mesh* mesh;
	int i, j, lasttex;

	if (textures != NULL)
		glEnable(GL_TEXTURE_2D);

	mesh = &rsm->getMesh(0);

	lasttex = -1;
	if (textures != NULL) {
		lasttex = 0;
		glBindTexture(GL_TEXTURE_2D, textures[lasttex]);
	}
	glBegin(GL_TRIANGLES);
	// Draw each surface
	for (i = 0; i < mesh->surfaces.getCount(); i++) {
		surface = &mesh->surfaces[i];
		// Check if we have textures
		if (textures != NULL) {
			if (surface->texid != lasttex) {
				glEnd();
				lasttex = surface->texid;
				glBindTexture(GL_TEXTURE_2D, textures[lasttex]);
				glBegin(GL_TRIANGLES);
			}
		}
		for (j = 0; j < 3; j++) {
			if (textures != NULL) {
				// This is only useful if we have textures
				glTexCoord2f(mesh->texv[surface->tv[j]].v[1], mesh->texv[surface->tv[j]].v[2]);
			}
			glVertex3f(mesh->vecs[surface->sv[j]].v[0], mesh->vecs[surface->sv[j]].v[1], mesh->vecs[surface->sv[j]].v[2]);
		}
	}
	glEnd();
	// TODO: Draw children

	if (textures != NULL)
		glDisable(GL_TEXTURE_2D);

	return(true);
}
