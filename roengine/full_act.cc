#include "stdafx.h"

#include "roengine/full_act.h"

#include <GL/gl.h>
#include <GL/glu.h>

FullAct::FullAct() {
	m_act = NULL;
}

FullAct::FullAct(const FullAct& act) {
	m_act = act.m_act;
	m_textures = act.m_textures;
}

FullAct& FullAct::operator = (const FullAct& act) {
	m_act = act.m_act;
	m_textures = act.m_textures;
	return(*this);
}

void FullAct::Clear() {
	// TODO: is this =NULL safe?
	m_act = NULL;
	m_textures.clear();
}

FullAct::FullAct(const RO::ACT* a, const rogl::Texture::PointerCache& t) {
	m_act = a;
	m_textures = t;
}

bool FullAct::Load(const std::string& name, ROObjectCache& objects, FileManager& fm, TextureManager& tm) {
	std::string act_n;
	std::string spr_n;

	act_n = name + ".act";
	spr_n = name + ".spr";

	if (!objects.ReadACT(act_n, fm)) {
		return(false);
	}
	m_act = (RO::ACT*)objects[act_n];
	m_textures = tm.RegisterSPR(fm, objects, spr_n);
	if (m_textures.size() == 0)
		return(false);

	return(true);
}


const RO::ACT* FullAct::getAct() const { return(m_act); }
const RO::ACT* FullAct::operator -> () const { return(m_act); }
const rogl::Texture::PointerCache& FullAct::getTextures() const { return(m_textures); }


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

void DrawFullAct(const FullAct& act, float x, float y, int act_no, int pat_no, bool ext, const FullAct* parent, bool v_mirror, bool cross) {
#define CROSS_SIZE 10
	unsigned int spr;
	float w, h;
	float u[2], v[2];
	rogl::Texture::Pointer tp;

	if (parent != NULL) {
		const RO::ACT::Act& pact = parent->getAct()->getAct(act_no);
		const RO::ACT::Pat& ppat = pact[pat_no];

		x = x + ppat.ext_x;
		y = y + ppat.ext_y;
	}

	if (cross) {
		glColor3f(1, 0, 0);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_LINES);
		glVertex3f(x - CROSS_SIZE, y,0);
		glVertex3f(x + CROSS_SIZE, y,0);
		glVertex3f(x, y - CROSS_SIZE,0);
		glVertex3f(x, y + CROSS_SIZE,0);
		glEnd();
		glEnable(GL_TEXTURE_2D);
		glColor3f(1, 1, 1);
	}

	if (act.getAct() == NULL)
		return;

	const RO::ACT::Act& cact = act->getAct(act_no);
	const RO::ACT::Pat& cpat = cact[pat_no];

	for (spr = 0; spr < cpat.spr.size(); spr++) {
		if (cpat[spr].sprNo < 0)
			continue;
		const RO::ACT::Spr& cspr = cpat[cpat[spr].sprNo];

		tp = act.getTextures()[cpat[spr].sprNo];
		if (!tp.isValid())
			return;

		if (cspr.w > 0) {
			w = (float)cspr.w;
			h = (float)cspr.h;
		}
		else {
			w = (float)tp->getWidth();
			h = (float)tp->getHeight();
		}

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
	}
}
