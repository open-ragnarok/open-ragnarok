/* $Id$ */
#include "stdafx.h"

#include "rsm_object.h"

#include <math.h>

RsmObject::RsmObject(const RO::RSM* o, const RO::RSW::Model* mdl) : GLObject() {
	rsm = o;
	m_time = 0;

	is_static = true;
	for (unsigned int i = 0; i < o->getMeshCount(); i++) {
		if (o->getMesh(i).getFrameCount() != 0)
			is_static = false;
	}
	model = mdl;
	if (model != NULL) {
		setPos(model->data->pos[0], -model->data->pos[1], model->data->pos[2]);
		setRot(model->data->rot[0], model->data->rot[1], model->data->rot[2]);
		setScale(model->data->scale[0], -model->data->scale[1], model->data->scale[2]);
	}
}

RsmObject::~RsmObject() {
	textures.clear();
}

bool RsmObject::loadTextures(TextureManager& tm, FileManager& fm) {
	unsigned int i;
	Texture::Pointer tex;
	std::string texname;

	for (i = 0; i < rsm->getTextureCount(); i++) {
		texname = "texture\\";
		texname += rsm->getTexture(i);
		tex = tm.Register(fm, texname);
		textures.add(tex);
	}

	return(true);
}

void RsmObject::DrawBoundingBox(const RO::RSM::BoundingBox& box) {
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

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_SHORT, lines);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void RsmObject::CalcRotFrame(const RO::RSM::Mesh& mesh, float* Ori, int& time) const {
	// Borrowed from RagCam (95%)

	int i;
	int current = 0;
	int next;
	float t;
	float x, y, z, w;
	//float q[4], q1[4], q2[4];
	//char buffer[1024];

	for (i = 0; i < mesh.frames.getCount(); i++) {
		if (time < mesh.frames[i].time) {
			current = i-1;
			break;
		}
	}

	next = current + 1;
	if (next == mesh.frames.getCount())
		next = 0;

	t = (time - mesh.frames[current].time) / (float)(mesh.frames[next].time - mesh.frames[current].time);

	//for (i = 0; i < 4; i++) {
	//	q[i] = frames[current].orientation[i] * (1-t) + t * frames[next].orientation[i];
	//}

	x = mesh.frames[current].rot[0] * (1-t) + t * mesh.frames[next].rot[0];
	y = mesh.frames[current].rot[1] * (1-t) + t * mesh.frames[next].rot[1];
	z = mesh.frames[current].rot[2] * (1-t) + t * mesh.frames[next].rot[2];
	w = mesh.frames[current].rot[3] * (1-t) + t * mesh.frames[next].rot[3];

	float norm;
	norm = sqrt(x*x+y*y+z*z+w*w);

	//for (i = 0; i < 4; i++)
	//	q[i] /= norm;
	x /= norm;
	y /= norm;
	z /= norm;
	w /= norm;

	// First row
	Ori[ 0] = 1.0f - 2.0f * ( y * y + z * z ); 
	Ori[ 1] = 2.0f * (x * y + z * w);
	Ori[ 2] = 2.0f * (x * z - y * w);
	Ori[ 3] = 0.0f;  

	// Second row
	Ori[ 4] = 2.0f * ( x * y - z * w );  
	Ori[ 5] = 1.0f - 2.0f * ( x * x + z * z ); 
	Ori[ 6] = 2.0f * (z * y + x * w );  
	Ori[ 7] = 0.0f;  

	// Third row
	Ori[ 8] = 2.0f * ( x * z + y * w );
	Ori[ 9] = 2.0f * ( y * z - x * w );
	Ori[10] = 1.0f - 2.0f * ( x * x + y * y );  
	Ori[11] = 0.0f;  

	// Fourth row
	Ori[12] = 0;  
	Ori[13] = 0;  
	Ori[14] = 0;  
	Ori[15] = 1.0f;

	/*
	//GLfloat two_x = q[0] * 2.0;
	//GLfloat two_y = q[1] * 2.0;
	//GLfloat two_z = q[2] * 2.0;

	Ori[0] = 1.0 - q[1] * 2.0; * q[1] - q[2] * 2.0 * q[2];
	Ori[1] = q[0] * 2.0 * q[1];
	Ori[2] = two_z * q[0];
	Ori[3] = 0.0;

	Ori[4] = two_x * q[1];
	Ori[5] = 1.0 - two_x * q[0] - two_z * q[2];
	Ori[6] = two_y * q[2];
	Ori[7] = 0.0;

	Ori[8] = two_z * q[0];
	Ori[9] = two_y * q[2];
	Ori[10] = 1.0 - two_x * q[0] - two_y * q[1];
	Ori[11] = 0.0;

	Ori[12] = 0.0;
	Ori[13] = 0.0;
	Ori[14] = 0.0;
	Ori[15] = 1.0;
	*/

	//printf("time: %d\tcurframe: %d\tcframetime: %d\tnframetime: %d\r", time, current, mesh.frames[current].time, mesh.frames[next].time);

	if (time >= mesh.frames[mesh.frames.getCount() - 1].time)
		time -= mesh.frames[mesh.frames.getCount() - 1].time;
}

void RsmObject::DrawMesh(unsigned int meshid) {
	int i, j, lasttex;
	const RO::RSM::Mesh& mesh = rsm->getMesh(meshid);
	//const RO::RSM::BoundingBox& box = mesh.getBoundingBox();
	const RO::RSM::BoundingBox& box = rsm->getBoundingBox();

	lasttex = -1;

	float mat[16];
	mat[0] = mesh.transf.mat33[0];
	mat[1] = mesh.transf.mat33[1];
	mat[2] = mesh.transf.mat33[2];
	mat[3] = 0;

	mat[4] = mesh.transf.mat33[3];
	mat[5] = mesh.transf.mat33[4];
	mat[6] = mesh.transf.mat33[5];
	mat[7] = 0;

	mat[8] = mesh.transf.mat33[6];
	mat[9] = mesh.transf.mat33[7];
	mat[10] = mesh.transf.mat33[8];
	mat[11] = 0;

	mat[12] = 0;
	mat[13] = 0;
	mat[14] = 0;
	mat[15] = 1;

	/*

	The order of things:
	* Calculate frames (TBD)
	* Scale
	* Translate
	* Rotate
	* Push
	** Translate
	** MultMatrix
	** Draw
	* Pop
	* Children

	 */

	float Ori[16];
	if (mesh.frames.getCount() > 0) {
		CalcRotFrame(mesh, Ori, m_time);
	}

	glScalef(mesh.transf.scale.c.x, mesh.transf.scale.c.y, mesh.transf.scale.c.z);

	if (mesh.is_main) {
		if (mesh.is_only)
			glTranslatef(0.0, -box.max.v[1] + box.range.v[1], 0.0);
		else
			glTranslatef(-box.range.v[0], -box.max.v[1], -box.range.v[2]);
	}
	else {
		glTranslatef(mesh.transf.translate2.c.x, mesh.transf.translate2.c.y, mesh.transf.translate2.c.z);
	}

	if (mesh.frames.getCount() == 0)
		glRotatef(mesh.transf.rot_angle * 180.0f / 3.14159f,
			mesh.transf.rot_vector.c.x, 
			mesh.transf.rot_vector.c.y, 
			mesh.transf.rot_vector.c.z);
	else {
		glMultMatrixf(Ori);
	}

	glPushMatrix();

	if (mesh.is_main && mesh.is_only) {
		glTranslatef(-box.range.v[0], -box.range.v[1], -box.range.v[2]);
	}
	else {
		glTranslatef(mesh.transf.translate1.c.x, mesh.transf.translate1.c.y, mesh.transf.translate1.c.z);
	}

	glMultMatrixf(mat);

#if 0
	glColor3f(1,0,1);
	DrawBoundingBox(box);
	glColor3f(1,1,1);
#endif

	glBegin(GL_TRIANGLES);
	// Draw each surface
	for (i = 0; i < mesh.surfaces.getCount(); i++) {
		const RO::RSM::Surface& surface = mesh.surfaces[i];
		if (surface.texid != lasttex) {
			glEnd();
			lasttex = surface.texid;
			textures[lasttex].Activate();
			//glBindTexture(GL_TEXTURE_2D, textures[lasttex]);
			glBegin(GL_TRIANGLES);
		}
		for (j = 0; j < 3; j++) {
			glTexCoord2f(mesh.texv[surface.tv[j]].v[1], 1.0f - mesh.texv[surface.tv[j]].v[2]);
			glVertex3f(mesh.vecs[surface.sv[j]].c.x, mesh.vecs[surface.sv[j]].c.y, mesh.vecs[surface.sv[j]].c.z);
		}
	}
	glEnd();

	glPopMatrix();

	for (unsigned int i = 0; i < rsm->getMeshCount(); i++) {
		if (i == meshid)
			continue;
		if (strcmp(rsm->getMesh(i).header.parent, mesh.header.name) == 0) {
			glPushMatrix();
			DrawMesh(i);
			glPopMatrix();
		}

	}
}

void RsmObject::Draw() {
	if (is_static) {
		if (glIsList(rsm_gl)) {
			glCallList(rsm_gl);
		}
		else {
			rsm_gl = glGenLists(1);
			glNewList(rsm_gl, GL_COMPILE_AND_EXECUTE);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.90f);
			DrawMesh(0);
			glDisable(GL_ALPHA_TEST);
			glDisable(GL_TEXTURE_2D);
			glEndList();
		}
	}
	else {
		m_time += m_tickdelay * 10;
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.90f);
		DrawMesh(0);
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_2D);
	}
}

bool RsmObject::isInFrustum(const Frustum& f) const {
	if (model == NULL)
		return(true);
	return(f.ModelVisible(rsm, model));
}

Texture::Pointer RsmObject::getTexture(unsigned int i) {
	return(textures[i]);
}
