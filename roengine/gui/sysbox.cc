/* $Id: sysbox.cc $ */
#include "stdafx.h"

#include "roengine/gui/sysbox.h"
#include "roengine/gui/gui.h"

namespace GUI {

SysBox::SysBox(Element* parent, CacheManager& cache) : Element(parent) {
}

SysBox::SysBox(Element* parent, const TiXmlElement* node, CacheManager& cache) : Element(parent, node, cache) {
	if (node != NULL)
		ParseFromXml(node, cache);

	if (!tex_lu.Valid())
		tex_lu = LoadTexture("sysbox_lu.bmp", cache);
	if (!tex_lm.Valid())
		tex_lm = LoadTexture("sysbox_lm.bmp", cache);
	if (!tex_ld.Valid())
		tex_ld = LoadTexture("sysbox_ld.bmp", cache);

	if (!tex_mu.Valid())
		tex_mu = LoadTexture("sysbox_mu.bmp", cache);
	if (!tex_mm.Valid())
		tex_mm = LoadTexture("sysbox_bg.bmp", cache);
	if (!tex_md.Valid())
		tex_md = LoadTexture("sysbox_md.bmp", cache);

	if (!tex_ru.Valid())
		tex_ru = LoadTexture("sysbox_ru.bmp", cache);
	if (!tex_rm.Valid())
		tex_rm = LoadTexture("sysbox_rm.bmp", cache);
	if (!tex_rd.Valid())
		tex_rd = LoadTexture("sysbox_rd.bmp", cache);
}

SysBox::~SysBox() {} 

void SysBox::Draw(unsigned int delay) {
//	float x = 0;
//	float y = 0;

	// Sanity check
	if (!tex_lu.Valid())
		return;
	if (!tex_lm.Valid())
		return;
	if (!tex_ld.Valid())
		return;

	if (!tex_mu.Valid())
		return;
	if (!tex_mm.Valid())
		return;
	if (!tex_md.Valid())
		return;

	if (!tex_ru.Valid())
		return;
	if (!tex_rm.Valid())
		return;
	if (!tex_rd.Valid())
		return;

	WindowSeq(0, 0, (float)w, (float)h, tex_lu, tex_lm, tex_ld,  tex_mu, tex_mm, tex_md, tex_ru, tex_rm, tex_rd);
}

}