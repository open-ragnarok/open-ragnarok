#include "stdafx.h"

#include "roengine/full_act.h"

#include <GL/gl.h>
#include <GL/glu.h>

FullAct::FullAct() {
	m_act = NULL;
}

FullAct::FullAct(const FullAct& act) {
	m_act = act.m_act;
	m_spr = act.m_spr;
}

FullAct& FullAct::operator = (const FullAct& act) {
	m_act = act.m_act;
	m_spr = act.m_spr;
	return(*this);
}

void FullAct::Clear() {
	// TODO: is this =NULL safe?
	m_act = NULL;
	m_spr.release();
}

FullAct::FullAct(const ro::ACT* a, const rogl::SprGL& spr) {
	m_act = a;
	m_spr = spr;
}

bool FullAct::Load(const std::string& name, CacheManager& cache, const ro::PAL* pal) {
	ROObjectCache& objects = cache.getROObjects();
	FileManager& fm = cache.getFileManager();
	TextureManager& tm = cache.getTextureManager();

	std::string act_n;
	std::string spr_n;

	act_n = name + ".act";
	spr_n = name + ".spr";

	if (!objects.ReadACT(act_n, fm)) {
		return(false);
	}

	if (!cache.getSprGLObjects().exists(spr_n)) {
		if (!cache.getSprGLObjects().Load(spr_n, objects, fm, pal))
			return(false);
	}

	m_spr = *cache.getSprGLObjects().get(spr_n);
	m_act = (ro::ACT*)objects[act_n];

	return(true);
}


const ro::ACT* FullAct::getAct() const { return(m_act); }
const ro::ACT* FullAct::operator -> () const { return(m_act); }
const sdle::Texture& FullAct::getTextures() const { return(m_spr.getTexture()); }

void DrawActCross(float x, float y, float size) {
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	glVertex3f(x - size, y,0);
	glVertex3f(x + size, y,0);
	glVertex3f(x, y - size,0);
	glVertex3f(x, y + size,0);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

const rogl::SprGL& FullAct::getSpr() const {
	return(m_spr);
}

void DrawFullAct(const FullAct& act, float x, float y, int act_no, int pat_no, bool ext, const FullAct* parent, bool v_mirror, bool cross, float opacity) {
#define CROSS_SIZE 10
	unsigned int spr;
	//float w, h;
	//float u[2], v[2];
	// rogl::Texture::Pointer tp;
	
	if (parent != NULL) {
		if (parent->getAct() != NULL) {
		//	const ro::ACT::Action& pact = parent->getAct()->getAction(act_no);
			const ro::ACT::Motion& pmot = parent->getAct()->getMotion(act_no, pat_no);

			if (pmot.attachPoints.size() > 0) {
				x += pmot.attachPoints[0].x;
				y += pmot.attachPoints[0].y;
			}
		}
	}

	if (cross) {
		glColor3f(1, 0, 0);
		DrawActCross(x, y, CROSS_SIZE);
		glColor3f(1, 1, 1);
	}

	if (act.getAct() == NULL)
		return;

	const ro::ACT::Motion& cmot = act->getMotion(act_no, pat_no);

	for (spr = 0; spr < cmot.getClipCount(); spr++) {
//		act.getSpr().Draw(cmot, spr, x, y, v_mirror, ext);
		act.getSpr().Draw(cmot, spr, x, y, v_mirror, ext, false, opacity);

/*
		if (cpat[spr].sprNo < 0)
			continue;
		const ro::ACT::Spr& cspr = cpat[cpat[spr].sprNo];

		tp = act.getTextures()[cpat[spr].sprNo];

		w *= cpat[spr].xMag;
		h *= cpat[spr].yMag;

		if (cspr.mirrorOn != 0)
			w = -w;

		u[0] = 0.0f;
		u[1] = tp->getMaxU();

		v[0] = 0.0f;
		v[1] = tp->getMaxV();
		
		if (v_mirror) {
			float aux = v[0];
			v[0] = v[1];
			v[1] = aux;
		}

		x += cpat[0].x;
		y += cpat[0].y;

		if (ext) {
			x -= cpat.ext_x;
			y -= cpat.ext_y;
		}

		tp.Activate();

		glBegin(GL_QUADS);
		glTexCoord2f(u[0], v[0]);
		glVertex3f(x - w/2, y - h/2, 0);
		glTexCoord2f(u[0], v[1]);
		glVertex3f(x - w/2, y + h/2, 0);
		glTexCoord2f(u[1], v[1]);
		glVertex3f(x + w/2, y + h/2, 0);
		glTexCoord2f(u[1], v[0]);
		glVertex3f(x + w/2, y - h/2, 0);
		glEnd();
*/
	}
}
