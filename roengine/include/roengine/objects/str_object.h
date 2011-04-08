#ifndef __STR_OBJECT_H
#define __STR_OBJECT_H

#include "ro/types/str.h"

#include "../gl_object.h"
#include "roengine/cache_manager.h"

#include <vector>

class StrObject : public GLObject {
#pragma pack(push,1)
/*	struct Color {
		float r, g, b, a;
		inline operator float* () { return(&r); }
		inline operator const float* () const { return(&r); }
	};

	struct Position {
		float x;
		float y;
		inline operator float* () { return &x; }
		inline operator const float* () const { return &x; }
	};

	struct TVertex {
		float u, v;
		inline operator float* () { return &u; }
		inline operator const float* () const { return &u; }
	};

	struct Rect {
		float x[4];
		float y[4];
	};*/

	struct Frame {
		ro::STR::Position pos;
		ro::STR::TVertex uv[2];
		ro::STR::TVertex uv2[2];
		ro::STR::Rect rect;
	//	float textureid;
		sdle::Texture *texture;
		float rotation;
		ro::STR::Color color;
	//	BlendMode srcblend;
	//	BlendMode dstblend;
		unsigned int srcblend;
		unsigned int dstblend;
		unsigned int mtpreset;
	};

	struct Layer {
	//	Arr<Texture> textures;
	//	TextureCache textures;
		std::vector<sdle::Texture> textures;
		std::vector<Frame> frames;
	};
#pragma pack(pop)

protected:
	unsigned int curframe;
	int m_time;

	ro::Arr<Layer> m_layers;
	const ro::STR *m_str;

	int m_lastframe, m_lastaniframe;
	int *m_lastnum, *m_lastaninum;

	void Billboard();
	void Window(float x, float y, const sdle::Texture& tex, bool mirrorX = false, bool mirrorY = false);
//	virtual void DrawStr(const ro::STR::Motion& mot, sdle::Texture& t);

public:
	StrObject();
	StrObject(const StrObject&);
	StrObject(const ro::STR*);
	virtual ~StrObject();

	bool open(CacheManager& cm, const ro::STR* str);
	// Deletes all info
	void release();

	virtual void Draw();
};

#endif /* __STR_OBJECT_H */
