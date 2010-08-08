/* $Id$ */
#include "stdafx.h"

#include "roengine/objects/rsm_object.h"

#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>

RsmObject::RsmObject(const ro::RSM* o, const ro::RSW::ModelObject* mdl) : GLObject() {
	rsm = o;
	m_time = 0;

	is_static = true;
	for (unsigned int i = 0; i < o->getNodeCount(); i++) {
		if (o->getNode(i).rotKeyframes.size() != 0)
			is_static = false;
	}
	model = mdl;
	if (model != NULL) {
		setPos(model->pos[0], -model->pos[1], model->pos[2]);
		setRot(model->rot[0], model->rot[1], model->rot[2]);
		setScale(model->scale[0], -model->scale[1], model->scale[2]);
	}
}

RsmObject::~RsmObject() {
	textures.clear();
}

bool RsmObject::loadTextures(CacheManager& cm) {
	unsigned int i;
	sdle::Texture tex;
	std::string texname;

	TextureManager& tm = cm.getTextureManager();
	FileManager& fm = cm.getFileManager();

	for (i = 0; i < rsm->getTextureCount(); i++) {
		texname = "texture\\";
		texname += rsm->getTexture(i);
		tex = tm.Register(fm, texname);
		textures.add(tex);
	}

	return(true);
}

void RsmObject::DrawBoundingBox() const {
	// TODO
#if 1

#if 1 // Use BoundingBox
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
#else // Use VolumeBox
	if (rsm->getVolumeBoxCount() == 0)
		return;
	const ro::RSM::VolumeBox& box = rsm->getVolumeBox(0);
	Vector3f max, min;
	max.setX(box.pos.x + box.size.x / 2.0f);
	max.setY(box.pos.y + box.size.y / 2.0f);
	max.setZ(box.pos.z + box.size.z / 2.0f);
	min.setX(box.pos.x - box.size.x / 2.0f);
	min.setY(box.pos.y - box.size.y / 2.0f);
	min.setZ(box.pos.z - box.size.z / 2.0f);
	float vertices[8][3] = {
		{ max.getX(), max.getY(), max.getZ() }, // 0
		{ max.getX(), max.getY(), min.getZ() }, // 1
		{ min.getX(), max.getY(), min.getZ() }, // 2
		{ min.getX(), max.getY(), max.getZ() }, // 3
		{ max.getX(), min.getY(), max.getZ() }, // 4
		{ max.getX(), min.getY(), min.getZ() }, // 5
		{ min.getX(), min.getY(), min.getZ() }, // 6
		{ min.getX(), min.getY(), max.getZ() }  // 7
	};
#endif

	static unsigned short lines[12][2] = {
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
	glLineWidth(2);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_SHORT, lines);
	glDisableClientState(GL_VERTEX_ARRAY);
#endif
}

void RsmObject::CalcRotFrame(const ro::RSM::Node& node, float* Ori, int& frame) const {
	// Borrowed from RagCam (95%)

	int i;
	int current = 0;
	int next;
	float t;
	float x, y, z, w;
	//float q[4], q1[4], q2[4];
	//char buffer[1024];

	for (i = 0; i < (int)node.rotKeyframes.size(); i++) {
		if (frame < node.rotKeyframes[i].frame) {
			current = i-1;
			break;
		}
	}

	next = current + 1;
	if (next == (int)node.rotKeyframes.size())
		next = 0;

	t = (frame - node.rotKeyframes[current].frame) / (float)(node.rotKeyframes[next].frame - node.rotKeyframes[current].frame);

	x = node.rotKeyframes[current].qx * (1-t) + t * node.rotKeyframes[next].qx;
	y = node.rotKeyframes[current].qy * (1-t) + t * node.rotKeyframes[next].qy;
	z = node.rotKeyframes[current].qz * (1-t) + t * node.rotKeyframes[next].qz;
	w = node.rotKeyframes[current].qw * (1-t) + t * node.rotKeyframes[next].qw;

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

	//printf("time: %d\tcurframe: %d\tcframetime: %d\tnframetime: %d\r", time, current, mesh.frames[current].time, mesh.frames[next].time);

//	if (frame >= node.rotKeyframes[node.rotKeyframes.size() - 1].frame)
//		frame -= node.rotKeyframes[node.rotKeyframes.size() - 1].frame;
	frame += node.rotKeyframes[node.rotKeyframes.size() - 1].frame / 100;
	if (frame >= node.rotKeyframes[node.rotKeyframes.size() - 1].frame)
		frame = 0;
/*	frame -= 33;
	if (frame >= node.rotKeyframes[current].frame)
	{
		frame = 0;
		current++;
		if (current >= node.rotKeyframes.size())
			current = 0;
	}*/
}

void RsmObject::DrawMesh(unsigned int meshid) {
	int i, j, lasttex;
	const ro::RSM::Node& node = rsm->getNode(meshid);
	//const ro::RSM::BoundingBox& box = mesh.getBoundingBox();
	const ro::RSM::BoundingBox& box = rsm->getBoundingBox();

	lasttex = -1;

	float mat[16];
	mat[0] = node.offsetMT[0];
	mat[1] = node.offsetMT[1];
	mat[2] = node.offsetMT[2];
	mat[3] = 0;

	mat[4] = node.offsetMT[3];
	mat[5] = node.offsetMT[4];
	mat[6] = node.offsetMT[5];
	mat[7] = 0;

	mat[8] = node.offsetMT[6];
	mat[9] = node.offsetMT[7];
	mat[10] = node.offsetMT[8];
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
	if (node.rotKeyframes.size() > 0) {
		CalcRotFrame(node, Ori, m_time);
	}

	glScalef(node.scale.x, node.scale.y, node.scale.z);

	if (node.is_main) {
		if (node.is_only) {
				glTranslatef(0.0, -box.max[1] + box.offset[1], 0.0);
//				glTranslatef(0.0, -box.max[1], 0.0);
//				glTranslatef(0.0, box.max[1] + box.offset[1], 0.0);
//				glTranslatef(0.0, 0, 0.0);
//				glTranslatef(0.0, box.max[1]-box.offset[1], 0.0);
		/*	if ((box.max[1] >= box.max[0]) && (box.max[1] >= box.max[2]))
				glTranslatef(0.0, -box.max[1] + box.offset[1], 0.0);
			else if ((box.max[0] >= box.max[1]) && (box.max[0] >= box.max[2]))
				glTranslatef(0.0, -box.max[0] + box.offset[0], 0.0);
			else if ((box.max[2] >= box.max[1]) && (box.max[2] >= box.max[0]))
				glTranslatef(0.0, -box.max[2] + box.offset[2], 0.0);*/
		/*	if ((box.max[1] >= box.max[0]) && (box.max[1] >= box.max[2]))
				glTranslatef(0.0, -box.max[1], 0.0);
			else if ((box.max[0] >= box.max[1]) && (box.max[0] >= box.max[2]))
				glTranslatef(0.0, -box.max[0], 0.0);
			else if ((box.max[2] >= box.max[1]) && (box.max[2] >= box.max[0]))
				glTranslatef(0.0, -box.max[2], 0.0);*/
		}
		else {
			glTranslatef(-box.offset[0], -box.max[1], -box.offset[2]);
		//	glTranslatef(-box.offset[0], box.max[1], -box.offset[2]);
		//	glTranslatef(0, -box.max[1], 0);
//			glTranslatef(0.0, -box.max[1] + box.offset[1], 0.0);
//			glTranslatef(node.pos.x, node.pos.y, node.pos.z);
		}
	}
	else
		glTranslatef(node.pos.x, node.pos.y, node.pos.z);

	if (node.rotKeyframes.size() == 0)
		glRotatef(node.rotangle * 180.0f / 3.14159f,
			node.rotaxis.x, 
			node.rotaxis.y, 
			node.rotaxis.z);
	else {
		glMultMatrixf(Ori);
	}

	glPushMatrix();

	if (node.is_main && node.is_only) {
		//Primera diferencia
		glTranslatef(-box.offset[0], -box.offset[1], -box.offset[2]);
//		glTranslatef(-box.offset[0], box.max[1], -box.offset[2]);
//		glTranslatef(-box.offset[0], box.offset[1], -box.offset[2]);
	//	glTranslatef(-box.offset[0], -box.offset[0], -box.offset[0]);
	/*		if ((box.max[1] >= box.max[0]) && (box.max[1] >= box.max[2]))
		glTranslatef(-box.offset[0], -box.offset[1], -box.offset[2]);
			else if ((box.max[0] >= box.max[1]) && (box.max[0] >= box.max[2]))
		glTranslatef(-box.offset[0], -box.offset[0], -box.offset[2]);
			else if ((box.max[2] >= box.max[1]) && (box.max[2] >= box.max[0]))
		glTranslatef(-box.offset[0], -box.offset[2], -box.offset[2]);*/
		//		glTranslatef(0.0, -box.max[1] + box.offset[1], 0.0);
	//			glTranslatef(0.0, -box.max[1], 0.0);
	}
//	else if (!node.is_main)
	else if (!node.is_main || !node.is_only)
		glTranslatef(node.offsetMT[9], node.offsetMT[10], node.offsetMT[11]);

#if 0
	if (node.is_main)
	{
		glColor3f(1,0,1);
		DrawBoundingBox();
		glColor3f(1,1,1);
	}
#endif
//	glColor4f(1,1,1,1);

	glMultMatrixf(mat);


//	glColor4fv(rsm->getColor());

//	glDisable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);
//	glBegin(GL_TRIANGLE_STRIP);
	// Draw each surface
	for (i = 0; i < (int)node.faces.size(); i++) {
		const ro::RSM::Face& face = node.faces[i];
		if (face.texid != lasttex) {
			glEnd();
			lasttex = face.texid;
			textures[node.textures[lasttex]].Activate();
			glBegin(GL_TRIANGLES);
		}
		float f3Vector01[3], f3Vector12[3];
		f3Vector01[0] = node.vertices[face.vertidx[1]].x - node.vertices[face.vertidx[0]].x;
		f3Vector12[0] = node.vertices[face.vertidx[2]].x - node.vertices[face.vertidx[0]].x;

		f3Vector01[1] = node.vertices[face.vertidx[1]].y - node.vertices[face.vertidx[0]].y;
		f3Vector12[1] = node.vertices[face.vertidx[2]].y - node.vertices[face.vertidx[0]].y;

		f3Vector01[2] = node.vertices[face.vertidx[1]].z - node.vertices[face.vertidx[0]].z;
		f3Vector12[2] = node.vertices[face.vertidx[2]].z - node.vertices[face.vertidx[0]].z;
		float f3NormalVector[3] = { f3Vector01[1] * f3Vector12[2] - f3Vector01[2] * f3Vector12[1],
									f3Vector01[2] * f3Vector12[0] - f3Vector01[0] * f3Vector12[2],
									f3Vector01[0] * f3Vector12[1] - f3Vector01[1] * f3Vector12[0] };
		glNormal3fv( f3NormalVector );
		for (j = 0; j < 3; j++) {
			glTexCoord2f(node.tvertices[face.tvertidx[j]].u, 1.0f - node.tvertices[face.tvertidx[j]].v);
			glVertex3fv(node.vertices[face.vertidx[j]]);
		}
	}
	glEnd();

//	glColor4f(1,1,1,1);

	glPopMatrix();

	for (unsigned int i = 0; i < rsm->getNodeCount(); i++) {
		if (i == meshid)
			continue;
		if (strcmp(rsm->getNode(i).parentname, node.name) == 0) {
			glPushMatrix();
			DrawMesh(i);
			glPopMatrix();
		}
	}
}

void RsmObject::Draw() {
	if (m_frustum != NULL) {
	//	if (!isInFrustum(*m_frustum)) {
		if (isInFrustum(*m_frustum)) {
		//	return;
		//	glColor3f(1, 0, 0);
			glColor4f(1, 1, 1, 1);
		}
		else {
			glColor4f(1, 1, 1, 1);
		}
	}
#if 0
	if (m_frustum != NULL) {
		if (!isInFrustum(*m_frustum)) {
			float f[4];
			glGetFloatv(GL_CURRENT_COLOR, f);
			glColor3f(1, 0, 0);
			DrawBoundingBox();
			glColor4fv(f);
			//return;
		}
		else {
			DrawBoundingBox();
		}
	}
#endif

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
	//	m_time += m_tickdelay * 10;
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

sdle::Texture RsmObject::getTexture(unsigned int i) {
	return(textures[i]);
}
