#include "stdafx.h"

#include "roengine/objects/str_object.h"

StrObject::StrObject() : GLObject() {
	m_time = 0;

	m_lastframe = -1;
	m_lastaniframe = -1;
}

StrObject::StrObject(const StrObject& o) {
	m_time = 0;
	m_lastframe = -1;
	m_lastaniframe = -1;
}

StrObject::StrObject(const ro::STR* str) {
	m_str = str;

	m_time = 0;
	m_lastframe = -1;
	m_lastaniframe = -1;
}

StrObject::~StrObject() {
}

void StrObject::release() {
}

bool StrObject::open(CacheManager& cm, const ro::STR* str) {
	release();

	m_str = str;////

	m_lastnum = new int[str->getLayerCount()];
	m_lastaninum = new int[str->getLayerCount()];

	TextureManager& tm = cm.getTextureManager();
	FileManager& fm = cm.getFileManager();

	m_layers.resize(str->getLayerCount());
	for (unsigned int i = 0; i < m_layers.size(); i++) {
		m_lastnum[i] = -1;
		m_lastaninum[i] = -1;
		// generate the OpenGL texture
		sdle::Texture tex;
		std::string texname;

		for (unsigned int j = 0; j < str->getLayer(i).textures.size(); j++) {
			texname = "texture\\effect\\";
			texname += str->getLayer(i).textures[j].name;
			tex = tm.Register(fm, texname);
			m_layers[i].textures.push_back(tex);
		}

		m_layers[i].frames.resize(str->getMaxFrame());
		int lastframe = -1;
	/*	for (unsigned int j = 0; j < str->getLayer(i).frames.size(); j++) {
			ro::STR::Frame _frame = str->getLayer(i).frames[j];
			Frame &frame = m_layers[i].frames[_frame.number];

			frame.color = _frame.color;
			frame.pos = _frame.pos;
			memcpy(frame.uv, _frame.uv, sizeof(ro::STR::Frame));
			memcpy(frame.uv2, _frame.uv2, sizeof(ro::STR::Frame));
			frame.rect = _frame.rect;
			//frame.textureid = _frame.textureid;
			frame.texture = &m_layers[i].textures[(unsigned int)_frame.textureid];
			frame.rotation = _frame.rotation;
			frame.srcblend = _frame.srcblend;
			frame.dstblend = _frame.dstblend;
			frame.mtpreset = _frame.mtpreset;
		}*/
	}

	return(true);
}


void StrObject::Billboard() {
	// Cheat Cylindrical. Credits: http://www.lighthouse3d.com/opengl/billboarding/index.php3?billCheat1
	float modelview[16];
	int i,j;

	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

	float v;

	for( i=0; i<3; i+=2 ) 
	    for( j=0; j<3; j++ ) {
			if ( i==j ) {
				if (i == 0)
					v = scalex;
				else if (i == 1)
					v = scaley;
				else
					v = 1.0f;
				modelview[i*4+j] = v;
			}
			else {
				modelview[i*4+j] = 0.0;
			}
	    }

	glLoadMatrixf(modelview);
}

void StrObject::Window(float x, float y, const sdle::Texture& tex, bool mirrorX, bool mirrorY) {
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.90f);

	tex.Activate();

	float w = (float)tex.getWidth();
	float h = (float)tex.getHeight();

	float u, u1;
	float v, v1;

	if (!mirrorY) {
		v1 = 0.0f;
		v = tex.getMaxV();
	}
	else {
		v = 0.0f;
		v1 = tex.getMaxV();
	}

	if (!mirrorX) {
		u1 = 0.0f;
		u = tex.getMaxU();
	}
	else {
		u = 0.0f;
		u1 = tex.getMaxU();
	}

	glBegin(GL_QUADS);
	glTexCoord2f(u1, v);  glVertex3f(x - w/2, y,     0);
	glTexCoord2f(u1, v1); glVertex3f(x - w/2, y + h, 0);
	glTexCoord2f(u, v1);  glVertex3f(x + w/2, y + h, 0);
	glTexCoord2f(u, v);   glVertex3f(x + w/2, y,     0);
	glEnd();

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
}
/*
void StrObject::DrawStr(const ro::ACT::Motion& mot, sdle::Texture& t) {
	Billboard();
	Window((float)mot.getClip(0).x, (float)mot.getClip(0).y, t, (mot.getClip(0).mirrorOn == 1));
}*/

void StrObject::Draw() {
	if (m_str == NULL)
		return;

	int texidx = 0;

	m_time += m_tickdelay;
	while (m_time >= 32) {
		m_time -= 32;
		curframe++;
		if (curframe >= m_str->getMaxFrame()) {
			curframe = 0;
			for (int i = 0; i < m_str->getLayerCount(); i++) {
				m_lastnum[i] = -1;
				m_lastaninum[i] = -1;
			}
		}
	}

	for (int i = 0; i < m_str->getLayerCount(); i++) {
		const ro::STR::Layer &layer = m_str->getLayer(i);
	//	for (int j = m_lastnum[i] + 1; j < layer.frames.size(); j++) {
		for (int j = m_lastnum[i] + 1 + (m_lastaninum[i] == -1 ? 0 : 1); j < layer.frames.size(); j++) {
			//	const ro::STR::Frame &frame = layer.frames[j];
			if (layer.frames[j].number == curframe) {
				if (layer.frames[j].type == 0) {
					m_lastframe = curframe;
					m_lastaninum[i] = -1;
					m_lastnum[i] = j;
					break;
				}
				else if (layer.frames[j].type == 1) {
					m_lastaninum[i] = j;
					break;
				}
			}
		}
//		glPushMatrix();

		const GLenum blendmode[] = {
			GL_ZERO
			,GL_ZERO
			,GL_ONE
			,GL_SRC_COLOR
			,GL_ONE_MINUS_SRC_COLOR
			,GL_SRC_ALPHA
			,GL_ONE_MINUS_SRC_ALPHA
			,GL_DST_ALPHA
			,GL_ONE_MINUS_DST_ALPHA
			,GL_DST_COLOR
			,GL_ONE_MINUS_DST_COLOR
			,GL_SRC_ALPHA_SATURATE
			,GL_SRC_ALPHA // TODO: BSA
			,GL_ZERO // TODO: BiSA
		};
	/*	const GLenum blendmode[] = {
			GL_ZERO
			,GL_ZERO
			,GL_ONE
			,GL_SRC_COLOR
			,GL_ONE_MINUS_SRC_COLOR
			,GL_SRC_ALPHA
			,GL_ONE_MINUS_SRC_ALPHA
			,GL_ONE
			,GL_ONE_MINUS_DST_ALPHA
			,GL_DST_COLOR
			,GL_ONE_MINUS_DST_COLOR
			,GL_SRC_ALPHA_SATURATE
			,GL_ZERO // TODO: BSA
			,GL_ZERO // TODO: BiSA
		};*/

		if (m_lastnum[i] >= 0) {
			float currentcolor[4];
			glGetFloatv(GL_CURRENT_COLOR, currentcolor);

			const ro::STR::Frame &frame = layer.frames[m_lastnum[i]];
		/*	if (m_lastaninum[i] == -1 && frame.color.a > 0.0f) {
				glColor4ub((char)frame.color.r, (char)frame.color.g, (char)frame.color.b, (char)frame.color.a);


				glTranslatef(frame.pos.x, frame.pos.y, -0.1);
			//	glTranslatef(frame.pos.x - 550, frame.pos.y - 200, -150);
				int f = frame.rotation;
				glRotatef(frame.rotation / 2.8444f, 0, 0, 1);
			//	glScalef(0.15, -0.15, 1);

				m_layers[i].textures[(int)frame.textureid].Activate();

				glBlendFunc(blendmode[frame.srcblend], blendmode[frame.dstblend]);
				glEnable(GL_BLEND);
				glEnable(GL_TEXTURE_2D);

				glColorMask(1, 1, 1, 0);

				glBegin(GL_QUADS);
				glTexCoord2f(frame.uv[1].u, frame.uv[0].v);  glVertex3f(frame.rect.x[2], frame.rect.y[2],     0.01 * i);
				glTexCoord2f(frame.uv[0].u, frame.uv[0].v);  glVertex3f(frame.rect.x[3], frame.rect.y[3],     0.01 * i);
				glTexCoord2f(frame.uv[0].u, frame.uv[1].v);  glVertex3f(frame.rect.x[0], frame.rect.y[0],     0.01 * i);
				glTexCoord2f(frame.uv[1].u, frame.uv[1].v);  glVertex3f(frame.rect.x[1], frame.rect.y[1],     0.01 * i);
				glEnd();

				glColorMask(1, 1, 1, 1);

				glDisable(GL_BLEND);
				glDisable(GL_TEXTURE_2D);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}
			else {
				const ro::STR::Frame &aniframe = layer.frames[m_lastaninum[i]];
				const int anifactor = curframe - aniframe.number;
				glColor4ub((unsigned char)frame.color.r + aniframe.color.r * anifactor, (unsigned char)frame.color.g + aniframe.color.g * anifactor, (unsigned char)frame.color.b + aniframe.color.b * anifactor, (unsigned char)(frame.color.a + aniframe.color.a * anifactor));

			//	glTranslatef(50, 50, -30);
				glTranslatef(frame.pos.x + aniframe.pos.x * anifactor, (frame.pos.y + aniframe.pos.y * anifactor), -0.1);
			//	glTranslatef(frame.pos.x + aniframe.pos.x * anifactor - 550, (frame.pos.y + aniframe.pos.y * anifactor) - 200, -150);
				int f = frame.rotation;
				glRotatef(frame.rotation / 2.8444f + (aniframe.rotation / 2.8444f) * anifactor, 0, 0, 1);
			//	glScalef(.02, .02, 1);
			//	glScalef(0.15, -0.15, 1);

				m_layers[i].textures[frame.textureid].Activate();

				glBlendFunc(blendmode[frame.srcblend], blendmode[frame.dstblend]); 
				glEnable(GL_BLEND);
				glEnable(GL_TEXTURE_2D);

				glColorMask(1, 1, 1, 0);

				glBegin(GL_QUADS);
				glTexCoord2f(frame.uv[1].u, frame.uv[0].v);  glVertex3f(frame.rect.x[2] + aniframe.rect.x[2] * anifactor, frame.rect.y[2] + aniframe.rect.y[2] * anifactor,     0.01 * i);
				glTexCoord2f(frame.uv[0].u, frame.uv[0].v);  glVertex3f(frame.rect.x[3] + aniframe.rect.x[3] * anifactor, frame.rect.y[3] + aniframe.rect.y[3] * anifactor,     0.01 * i);
				glTexCoord2f(frame.uv[0].u, frame.uv[1].v);  glVertex3f(frame.rect.x[0] + aniframe.rect.x[0] * anifactor, frame.rect.y[0] + aniframe.rect.y[0] * anifactor,     0.01 * i);
				glTexCoord2f(frame.uv[1].u, frame.uv[1].v);  glVertex3f(frame.rect.x[1] + aniframe.rect.x[1] * anifactor, frame.rect.y[1] + aniframe.rect.y[1] * anifactor,     0.01 * i);
				glEnd();

				glColorMask(1, 1, 1, 1);

				glDisable(GL_BLEND);
				glDisable(GL_TEXTURE_2D);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			}*/
			if (frame.color.a > 0.0f) {
				unsigned char color[4];
				ro::STR::Position pos;
				float rotation;
				ro::STR::Rect rect;
				ro::STR::TVertex uv[2];
				int textureid;
				unsigned char srcblend, dstblend;

				for (int j = 0; j < 4; j++)
					color[j] = (unsigned char)frame.color[j];
				pos.x = frame.pos.x - 320;
				pos.y = frame.pos.y - 290;
				rotation = frame.rotation / 2.8444f;
				rect = frame.rect;
				for (int j = 0; j < 2; j++)
					uv[j] = frame.uv[j];
				srcblend = frame.srcblend;
				dstblend = frame.dstblend;
				textureid = frame.textureid;

				if (m_lastaninum[i] >= 0) {
					const ro::STR::Frame &aniframe = layer.frames[m_lastaninum[i]];
					const int anifactor = curframe - aniframe.number;

					for (int j = 0; j < 4; j++)
						color[j] += (unsigned char)aniframe.color[j] * anifactor;
					pos.x += aniframe.pos.x * anifactor;
					pos.y += aniframe.pos.y * anifactor;
					rotation += (aniframe.rotation / 2.8444f) * anifactor;
					for (int j = 0; j < 4; j++) {
						rect.x[j] += aniframe.rect.x[j] * anifactor;
						rect.y[j] += aniframe.rect.y[j] * anifactor;
					}
					for (int j = 0; j < 2; j++) {
						uv[j].u += aniframe.uv[j].u * anifactor;
						uv[j].v += aniframe.uv[j].v * anifactor;
					}
				}

				glPushMatrix();

				Billboard();

				glColor4ubv(color);
//				glColor4f(1,1,1,1);
				glTranslatef(pos.x, pos.y, -0.5);
				glRotatef(rotation, 0, 0, 1);
			//	glScalef(0.15, -0.15, 1);

				m_layers[i].textures[(int)textureid].Activate();

				glBlendFunc(blendmode[srcblend], blendmode[dstblend]);
				glEnable(GL_BLEND);
				glEnable(GL_TEXTURE_2D);
				glEnable(GL_ALPHA_TEST);
				glAlphaFunc(GL_GREATER, 0.0f);

				glColorMask(1, 1, 1, 0);

				glBegin(GL_QUADS);
				glTexCoord2f(uv[1].u, uv[0].v);  glVertex3f(rect.x[2], rect.y[2],     0.01 * i);
				glTexCoord2f(uv[0].u, uv[0].v);  glVertex3f(rect.x[3], rect.y[3],     0.01 * i);
				glTexCoord2f(uv[0].u, uv[1].v);  glVertex3f(rect.x[0], rect.y[0],     0.01 * i);
				glTexCoord2f(uv[1].u, uv[1].v);  glVertex3f(rect.x[1], rect.y[1],     0.01 * i);
				glEnd();

				glColorMask(1, 1, 1, 1);

				glDisable(GL_ALPHA_TEST);
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


				glColor4fv(currentcolor);

				glPopMatrix();
			}
		}

	//	glPopMatrix();
	}
}
