#include "stdafx.h"

#include "rogl/sprgl.h"
#include <math.h>


namespace rogl {

static const sprInfo g_emptyInfo = {0,0,0,0,0,0};

SprGL::SprGL() {
	m_palCount = 0;
	m_rgbaCount = 0;
	m_filtering = true;
}

SprGL::SprGL(const SprGL& s) {
	m_texture = s.m_texture;
	m_palCount = s.m_palCount;
	m_rgbaCount = s.m_rgbaCount;
	m_info = s.m_info;
	m_filtering = true;
}

SprGL::~SprGL() {
	release();
}

unsigned int SprGL::getFrameCount() const {
	return(m_info.size());
}

unsigned int SprGL::getIndex(int num, int type) const {
	switch (type) {
		case 0:
			// pal image
			if (num >= 0 && (unsigned int)num < m_palCount)
				return((unsigned int)num);
			break;
		case 1:
			// rgba image
			if (num >= 0 && (unsigned int)num < m_rgbaCount)
				return((unsigned int)num + m_palCount);
			break;
	}
	return((unsigned int)-1);
}

void SprGL::release() {
	m_texture = sdle::Texture();
	m_palCount = 0;
	m_rgbaCount = 0;
	m_info.clear();
}

sdle::Texture& SprGL::getTexture() {
	return(m_texture);
}

const sdle::Texture& SprGL::getTexture() const {
	return(m_texture);
}

const sprInfo& SprGL::getFrameInfo(unsigned int frame) const {
	if (frame < m_info.size())
		return(m_info[frame]);
	return(g_emptyInfo);
}

SprGL& SprGL::operator = (const SprGL& s) {
	release();
	m_texture = s.m_texture;
	m_palCount = s.m_palCount;
	m_rgbaCount = s.m_rgbaCount;
	m_info = s.m_info;
	return(*this);
}

bool SprGL::operator == (const SprGL& s) const {
	return(m_texture == s.m_texture);
}

bool SprGL::open(const ro::SPR* spr, const ro::PAL* pal) {
	unsigned int i;

	release();
	if (pal == NULL) {
		pal = spr->getPal();
	}
	if (pal == NULL && spr->getImageCount(ro::SPR::PalType) > 0){
		fprintf(stderr, "SprGL::open - pal is NULL (%u SPR pal images)\n", spr->getImageCount(ro::SPR::PalType));
		return(false);
	}

	m_palCount = spr->getImageCount(ro::SPR::PalType);
	m_rgbaCount = spr->getImageCount(ro::SPR::RgbaType);
	m_info.resize(spr->getImageCount(), g_emptyInfo);

	// calculate needed area
	unsigned int neededArea = 0;
	for (i = 0; i < m_info.size(); i++) {
		const ro::SPR::Image* img = spr->getImage(i);
		neededArea += img->width * img->height;
	}
	// calculate texture size (power of two)
	unsigned int textureSize;
	for (textureSize = 1; ; textureSize *= 2) {
		if (textureSize * textureSize < neededArea) {
			// too small
			continue;
		}

		bool tooSmall = false;
		unsigned int neededHeight = 0;
		unsigned int linespace = textureSize;
		unsigned int lineheight = 0;
		for (i = 0; i < m_info.size(); i++) {
			const ro::SPR::Image* img = spr->getImage(i);
			if (img->width > textureSize || neededHeight > textureSize) {
				tooSmall = true;
				break;
			}
			if (img->width > linespace) {
				// next line
				neededHeight += lineheight;
				linespace = textureSize;
				lineheight = 0;
			}
			if (lineheight < img->height) {
				lineheight = img->height;
			}
			linespace -= img->width;
		}
		neededHeight += lineheight;
		if (neededHeight > textureSize || tooSmall) {
			// too small
			continue;
		}
		// all images fit
		break;
	}

	// create texture image
	sdle::Image teximage(NULL, textureSize, textureSize, 32);
	unsigned int offx = 0;
	unsigned int offy = 0;
	unsigned int lineheight = 0;
	for (i = 0; i < m_info.size(); i++) {
		const ro::SPR::Image* img = spr->getImage(i);
		if (offx + img->width > textureSize) {
			// next line
			offx = 0;
			offy += lineheight;
			lineheight = 0;
		}
		if (lineheight < img->height) {
			lineheight = img->height;
		}

		// write data to image
		// TODO shouldn't this be using: left to right, bottom to top? [flaviojs]
		if (img->type == ro::SPR::PalType) {
			// left to right, top to bottom
			for (unsigned int x = 0; x < img->width; x++) {
				for (unsigned int y = 0; y < img->height; y++) {
					unsigned char palpos = img->data.pal[x + y * img->width];
					const ro::PAL::Color& color = pal->getColor(palpos);
					unsigned char* p = (unsigned char*)&teximage[4 * ((offx + x) + (offy + y) * textureSize)];
					p[0] = color.r;
					p[1] = color.g;
					p[2] = color.b;
					if (palpos == 0) {
						// invisible background index
						p[3] = 0;
					}
					else {
						p[3] = 0xff;
					}
				}
			}
		}
		else if (img->type == ro::SPR::RgbaType) {
			// left to right, bottom to top -> left to right, top to bottom
			for (unsigned int x = 0; x < img->width; x++) {
				for (unsigned int y = 0; y < img->height; y++) {
					const ro::SPR::Color& color = img->data.rgba[x + (img->height - y - 1) * img->width];
					unsigned char* p = (unsigned char*)&teximage[4 * ((offx + x) + (offy + y) * textureSize)];
					p[0] = color.r;
					p[1] = color.g;
					p[2] = color.b;
					p[3] = color.a;
				}
			}
		}
		// Setup info
		sprInfo& info = m_info[i];
		info.w = img->width;
		info.h = img->height;
		info.su = (float)(offx) / textureSize;
		info.eu = (float)(offx + img->width) / textureSize;
		info.sv = (float)(offy) / textureSize;
		info.ev = (float)(offy + img->height) / textureSize;

		/*
		printf("SPR Index %d\t size: (%d,%d)\t", i, m_info[i].w, m_info[i].h);
		printf("u: %.2f, %.2f\tv: %.2f, %.2f", m_info[i].su, m_info[i].eu, m_info[i].sv, m_info[i].ev);
		printf("\t i: %d, %d", ix, iy);
		printf("\n");
		*/

		offx += img->width;
	}

	// generate the OpenGL texture
	sdle::Texture::Root* root = new sdle::Texture::Root();
	if (!root->Create(&teximage)) {
//	if (!root->Create(&teximage, false)) {
		release();
		return(false);
	}
	m_texture = root;
	return(true);
}

void SprGL::Draw() const {
	Draw(0);
}

void SprGL::Draw(unsigned int idx, struct Rect rect, bool xmirror, bool ymirror, bool filtering, float opacity) const {
	if (idx >= m_info.size())
		return;

	float width = rect.dim.w;
	float height = rect.dim.h;
//	float sx = - width / 2 + rect.pos.x; // Start X Position
//	float sy = rect.pos.y; // Start Y Position
	float sx = rect.pos.x; // Start X Position
	float sy = rect.pos.y; // Start Y Position

	float u[2];
	float v[2];

	if (!xmirror) {
		u[0] = m_info[idx].su;
		u[1] = m_info[idx].eu;
	}
	else {
		u[1] = m_info[idx].su;
		u[0] = m_info[idx].eu;
	}

	if (!ymirror) {
		v[0] = m_info[idx].sv;
		v[1] = m_info[idx].ev;
	}
	else {
		v[1] = m_info[idx].sv;
		v[0] = m_info[idx].ev;
	}


	m_texture.Activate();
//	if (m_filtering != filtering) {
		if (!filtering) {
		//	m_filtering = false;
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		else {
		//	m_filtering = true;
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
//	}
	glEnable(GL_BLEND);
	glColor4f(1.0f, 1.0f, 1.0f, opacity);
	
	glBegin(GL_QUADS);
	glTexCoord2f(u[0], v[1]);	glVertex3f(sx, sy, 0.0f);
	glTexCoord2f(u[0], v[0]);	glVertex3f(sx, sy + height, 0.0f);
	glTexCoord2f(u[1], v[0]);	glVertex3f(sx + width, sy + height, 0.0f);
	glTexCoord2f(u[1], v[1]);	glVertex3f(sx + width, sy, 0.0f);
	glEnd();

	glDisable(GL_BLEND);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void SprGL::Draw(unsigned int idx, float width, float height, bool xmirror, bool ymirror) const {
	struct Rect r;
	r.pos.x = 0;
	r.pos.y = 0;

	r.dim.w = width;
	r.dim.h = height;

	Draw(idx, r, xmirror, ymirror);
}

void SprGL::Draw(unsigned int idx, bool xmirror) const {
	if (idx >= m_info.size())
		return;

	struct Rect r;
	r.pos.x = 0;
	r.pos.y = 0;

	r.dim.w = (float)m_info[idx].w;
	r.dim.h = (float)m_info[idx].h;

	Draw(idx, r, xmirror);
}

void SprGL::Draw(const ro::ACT::Motion& cmot, unsigned int clpno, float& x, float& y, bool v_mirror, bool ext, bool filtering, float opacity) const {
	const ro::ACT::SprClip& cspr = cmot.getClip(clpno);
	unsigned int idx = getIndex(cspr.sprNo, cspr.sprType);

	if (idx >= m_info.size())
		return;

	float w, h;
	w = (float)m_info[idx].w;
	h = (float)m_info[idx].h;

	// Magnitude..
	w *= cspr.xMag;
	h *= cspr.yMag;

	if (ext && cmot.attachPoints.size() > 0) {
		x -= cmot.attachPoints[0].x;
		y -= cmot.attachPoints[0].y;
	}
	struct Rect r;
	r.pos.x = x - ceil(w / 2) + cspr.x;
	r.pos.y = y - ceil(h / 2) + cspr.y;
	r.dim.w = w;
	r.dim.h = h;

	Draw(idx, r, (bool)cspr.mirrorOn, v_mirror, filtering, opacity);
}

}
