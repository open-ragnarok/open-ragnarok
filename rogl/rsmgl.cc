/* $Id$ */
#include "stdafx.h"

#include "rogl/rsmgl.h"

bool rogl::draw(const ro::RSM* rsm, const unsigned int* textures, int time) {
	if (rsm == NULL)
		return(false);

	drawMesh(0, rsm, textures);

	return(true);
}

bool rogl::drawBoundingBox(const ro::RSM* rsm) {
	// TODO
#if 0
	const ro::RSM::BoundingBox& box = rsm->getBoundingBox();

	float vertices[8][3] = {
		{ box.max.x, box.max.y, box.max.z }, // 0
		{ box.max.x, box.max.y, box.min.z }, // 1
		{ box.min.x, box.max.y, box.min.z }, // 2
		{ box.min.x, box.max.y, box.max.z }, // 3
		{ box.max.x, box.min.y, box.max.z }, // 4
		{ box.max.x, box.min.y, box.min.z }, // 5
		{ box.min.x, box.min.y, box.min.z }, // 6
		{ box.min.x, box.min.y, box.max.z }  // 7
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
#endif
	return(false);
}

bool rogl::drawMesh(const unsigned int& meshidx, const ro::RSM* rsm, const unsigned int* textures) {
	if (rsm == NULL)
		return(false);

	int i, j, lasttex;

	if (textures != NULL)
		glEnable(GL_TEXTURE_2D);

	const ro::RSM::Node& node = rsm->getNode(meshidx);

	lasttex = -1;
	if (textures != NULL) {
		lasttex = 0;
		glBindTexture(GL_TEXTURE_2D, textures[lasttex]);
	}
	glBegin(GL_TRIANGLES);
	// Draw each surface
	for (i = 0; i < (int)node.faces.size(); i++) {
		const ro::RSM::Face& face = node.faces[i];
		// Check if we have textures
		if (textures != NULL) {
			if (face.texid != lasttex) {
				glEnd();
				lasttex = face.texid;
				glBindTexture(GL_TEXTURE_2D, textures[lasttex]);
				glBegin(GL_TRIANGLES);
			}
		}
		for (j = 0; j < 3; j++) {
			if (textures != NULL) {
				// This is only useful if we have textures
				glTexCoord2fv(node.tvertices[face.tvertidx[j]]);
			}
			glVertex3fv(node.vertices[face.vertidx[j]]);
		}
	}
	glEnd();

	if (textures != NULL)
		glDisable(GL_TEXTURE_2D);

	return(true);
}
