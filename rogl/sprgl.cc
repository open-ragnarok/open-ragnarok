#include "stdafx.h"

#include "rogl/sprgl.h"
#include <gl/gl.h>


namespace rogl {

SprGL::SprGL() {
	m_info = NULL;
	m_framecount = 0;
}

SprGL::SprGL(const SprGL& s) {
	m_texture = s.m_texture;
	m_framecount = s.m_framecount;
	if (m_framecount > 0) {
		m_info = new sprInfo[m_framecount];
		for (unsigned int i = 0 ; i < m_framecount; i++) {
			m_info[i].su = s.m_info[i].su;
			m_info[i].eu = s.m_info[i].eu;
			m_info[i].sv = s.m_info[i].sv;
			m_info[i].ev = s.m_info[i].ev;
			m_info[i].w = s.m_info[i].w;
			m_info[i].h = s.m_info[i].h;
		}
	}
}

SprGL::~SprGL() {
	if (m_info != NULL)
		delete[] m_info;
}

unsigned int SprGL::getFrameCount() const {
	return(m_framecount);
}

void SprGL::release() {
	if (m_info != NULL)
		delete[] m_info;
	m_info = NULL;
	m_framecount = 0;
	m_texture = sdle::Texture();
}

sdle::Texture& SprGL::getTexture() {
	return(m_texture);
}

const sdle::Texture& SprGL::getTexture() const {
	return(m_texture);
}

const sprInfo& SprGL::getFrameInfo(unsigned int frame) const {
	return(m_info[frame]);
}

SprGL& SprGL::operator = (const SprGL& s) {
	release();

	m_texture = s.m_texture;
	m_framecount = s.m_framecount;
	if (m_framecount > 0) {
		m_info = new sprInfo[m_framecount];
		for (unsigned int i = 0 ; i < m_framecount; i++) {
			m_info[i].su = s.m_info[i].su;
			m_info[i].eu = s.m_info[i].eu;
			m_info[i].sv = s.m_info[i].sv;
			m_info[i].ev = s.m_info[i].ev;
			m_info[i].w = s.m_info[i].w;
			m_info[i].h = s.m_info[i].h;
		}
	}
	return(*this);
}


bool SprGL::open(const RO::SPR* spr) {
	unsigned int imageWidth = 256;
	unsigned int neededHeight = 0;

	unsigned int linespace = imageWidth;
	unsigned int lineheight = 0;

	unsigned int i;
	int pos;


	const RO::SPR::Image* img;
	const RO::SPR::Pal* pal;

	m_info = new sprInfo[spr->getImgCount()];
	m_framecount = spr->getImgCount();

	// Check our image size
	for (i = 0; i < m_framecount; i++) {
		img = spr->getFrame(i);
		if (img->w > imageWidth) {
			fprintf(stderr, "SPR frame too big!\n");
			return(false);
		}
		if (img->w > linespace) {
			// Too big to fit this line. Increase one line.
			linespace = imageWidth;
			neededHeight += lineheight;
			lineheight = 0;
		}
		if (img->h > lineheight) {
			lineheight = img->h;
		}
		linespace -= img->w;
	}
	neededHeight += lineheight;
	unsigned int imageHeight = 16;
	// Get the next power-of-two size
	while (imageHeight < neededHeight)
		imageHeight <<= 1;

	// Creates the Image

	// Corner coordinates for the current image coordinates inside the texture
	int ix = 0;
	int iy = 0;
	// Actual Coordinates of the pixel in the texture
	int px, py;
	// Pallete position
	unsigned char palpos;
	lineheight = 0;
	linespace = imageWidth;
	sdle::Image image(NULL, imageWidth, imageHeight, 32);

	for (i = 0; i < m_framecount; i++) {
		img = spr->getFrame(i);
		// Checks our position
		if (img->w > linespace) {
			// Too big to fit this line. Increase one line.
			linespace = imageWidth;
			iy += lineheight;
			ix = 0;
			lineheight = 0;
		}
		if (img->h > lineheight) {
			lineheight = img->h;
		}

		// Copy data to image
		for (unsigned int x = 0; x < img->w; x++) {
			for (unsigned int y = 0; y < img->h; y++) {
				px = ix + x;
				py = iy + y;
				pos = y * img->w + x;
				palpos = img->data[pos];
				pal = spr->getPal(palpos);
				// Write data to image
				image[4 * (px + py * imageWidth)] = pal->r;
				image[4 * (px + py * imageWidth) + 1] = pal->g;
				image[4 * (px + py * imageWidth) + 2] = pal->b;
				if (palpos == 0) {
					image[4 * (px + py * imageWidth) + 3] = 0;
				}
				else {
					image[4 * (px + py * imageWidth) + 3] = (unsigned char)0xff;
				}
			}
		}
		// Setup info
		m_info[i].w = img->w;
		m_info[i].h = img->h;
		m_info[i].su = ((float)ix) / imageWidth;
		m_info[i].eu = ((float)(ix + img->w)) / imageWidth;
		m_info[i].sv = ((float)iy) / imageHeight;
		m_info[i].ev = ((float)(iy + img->h)) / imageHeight;

		/*
		printf("SPR Index %d\t size: (%d,%d)\t", i, m_info[i].w, m_info[i].h);
		printf("u: %.2f, %.2f\tv: %.2f, %.2f", m_info[i].su, m_info[i].eu, m_info[i].sv, m_info[i].ev);
		printf("\t i: %d, %d", ix, iy);
		printf("\n");
		*/

		ix += img->w;
		linespace -= img->w;
	}

    // generate the OpenGL texture from the byte array
	sdle::Texture::Root* root = new sdle::Texture::Root();
    if (!root->Create(&image)) {
		release();
    	return(false);
    }
    m_texture = root;


	return(true);
}

void SprGL::Draw() const {
	Draw(0);
}

void SprGL::Draw(unsigned int idx, bool xmirror) const {
	if (idx >= m_framecount)
		return;

	float sx = -((float)m_info[idx].w) / 2;

	m_texture.Activate();
	glBegin(GL_QUADS);
	// TODO: Optimize this
	if (!xmirror) {
		glTexCoord2f(m_info[idx].su, m_info[idx].ev);	glVertex3f(sx, 0.0f, 0.0f);
		glTexCoord2f(m_info[idx].su, m_info[idx].sv);	glVertex3f(sx, (float)m_info[idx].h, 0.0f);
		glTexCoord2f(m_info[idx].eu, m_info[idx].sv);	glVertex3f(sx + (float)m_info[idx].w, (float)m_info[idx].h, 0.0f);
		glTexCoord2f(m_info[idx].eu, m_info[idx].ev);	glVertex3f(sx + (float)m_info[idx].w, 0.0f, 0.0f);
	}
	else {
		glTexCoord2f(m_info[idx].eu, m_info[idx].ev);	glVertex3f(sx, 0.0f, 0.0f);
		glTexCoord2f(m_info[idx].eu, m_info[idx].sv);	glVertex3f(sx, (float)m_info[idx].h, 0.0f);
		glTexCoord2f(m_info[idx].su, m_info[idx].sv);	glVertex3f(sx + (float)m_info[idx].w, (float)m_info[idx].h, 0.0f);
		glTexCoord2f(m_info[idx].su, m_info[idx].ev);	glVertex3f(sx + (float)m_info[idx].w, 0.0f, 0.0f);
	}
	glEnd();
}

void SprGL::Draw(const RO::ACT::Pat& cpat, unsigned int sprno, float& x, float& y, bool v_mirror, bool ext) const {
	if (cpat[sprno].sprNo < 0)
		return;

	const RO::ACT::Spr& cspr = cpat[cpat[sprno].sprNo];

	unsigned int idx = cspr.sprNo;

	if (idx >= m_framecount)
		return;

	float w, h;
	float u[2], v[2];
	w = (float)m_info[idx].w;
	h = (float)m_info[idx].h;

	u[0] = m_info[idx].su;
	u[1] = m_info[idx].eu;
	v[0] = m_info[idx].sv;
	v[1] = m_info[idx].ev;

	// Magnitude..
	w *= cspr.xMag;
	h *= cspr.yMag;

	if (cspr.mirrorOn != 0)
		w = -w;

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


	m_texture.Activate();

	glBegin(GL_QUADS);
	glTexCoord2f(u[0], v[0]); glVertex3f(x - w/2, y - h/2, 0);
	glTexCoord2f(u[0], v[1]); glVertex3f(x - w/2, y + h/2, 0);
	glTexCoord2f(u[1], v[1]); glVertex3f(x + w/2, y + h/2, 0);
	glTexCoord2f(u[1], v[0]); glVertex3f(x + w/2, y - h/2, 0);
	glEnd();

}


}