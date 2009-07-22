/* $Id$ */
#include "stdafx.h"

#include "rogl/rsmgl.h"

bool rogl::draw(const RO::RSM* rsm, const unsigned int* textures, int time) {
	if (rsm == NULL)
		return(false);

	drawMesh(0, rsm, textures);

	return(true);
}

bool rogl::drawBoundingBox(const RO::RSM* rsm) {
	const RO::RSM::BoundingBox& box = rsm->getBoundingBox();

	float vertices[8][3] = {
		{ box.max.c.x, box.max.c.y, box.max.c.z }, // 0
		{ box.max.c.x, box.max.c.y, box.min.c.z }, // 1
		{ box.min.c.x, box.max.c.y, box.min.c.z }, // 2
		{ box.min.c.x, box.max.c.y, box.max.c.z }, // 3
		{ box.max.c.x, box.min.c.y, box.max.c.z }, // 4
		{ box.max.c.x, box.min.c.y, box.min.c.z }, // 5
		{ box.min.c.x, box.min.c.y, box.min.c.z }, // 6
		{ box.min.c.x, box.min.c.y, box.max.c.z }  // 7
	};

	// No push-pop needed, since de don't do any transformations, it's transparent for whoever is calling.
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

#if 0 /* use the faces */
	unsigned short faces[6][4] = {
		{ 0, 1, 2, 3 },
		{ 0, 1, 5, 4 },
		{ 1, 2, 6, 5 },
		{ 2, 3, 7, 6 },
		{ 0, 3, 7, 4 },
		{ 4, 5, 6, 7 }
	};

	glDrawElements(GL_LINES, 24, GL_UNSIGNED_SHORT, faces);

#else /* use the lines */
	unsigned short lines[12][2] = {
		{ 0, 1 },
		{ 1, 2 },
		{ 2, 3 },
		{ 3, 0 },
		{ 0, 4 },
		{ 1, 5 },
		{ 2, 6 },
		{ 3, 7 },
		{ 4, 5 },
		{ 5, 6 },
		{ 6, 7 },
		{ 7, 4 }
	};

	glDrawElements(GL_LINES, 24, GL_UNSIGNED_SHORT, lines);
#endif

	glDisableClientState(GL_VERTEX_ARRAY);

	return(true);
}

bool rogl::drawMesh(const unsigned int& meshidx, const RO::RSM* rsm, const unsigned int* textures) {
	if (rsm == NULL)
		return(false);

	int i, j, lasttex;

	if (textures != NULL)
		glEnable(GL_TEXTURE_2D);

	const RO::RSM::Mesh& mesh = rsm->getMesh(meshidx);

	lasttex = -1;
	if (textures != NULL) {
		lasttex = 0;
		glBindTexture(GL_TEXTURE_2D, textures[lasttex]);
	}
	glBegin(GL_TRIANGLES);
	// Draw each surface
	for (i = 0; i < mesh.surfaces.getCount(); i++) {
		const RO::RSM::Surface& surface = mesh.surfaces[i];
		// Check if we have textures
		if (textures != NULL) {
			if (surface.texid != lasttex) {
				glEnd();
				lasttex = surface.texid;
				glBindTexture(GL_TEXTURE_2D, textures[lasttex]);
				glBegin(GL_TRIANGLES);
			}
		}
		for (j = 0; j < 3; j++) {
			if (textures != NULL) {
				// This is only useful if we have textures
				glTexCoord2f(mesh.texv[surface.tv[j]].v[1], mesh.texv[surface.tv[j]].v[2]);
			}
			glVertex3fv(mesh.vecs[surface.sv[j]].v);
		}
	}
	glEnd();

	if (textures != NULL)
		glDisable(GL_TEXTURE_2D);

	return(true);
}
