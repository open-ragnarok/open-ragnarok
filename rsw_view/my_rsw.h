/* $Id$ */
#ifndef __MY_RSW_H
#define __MY_RSW_H

#include "ro/ro.h"
#include "rogl/rogl.h"

#include "image_bmp.h"
//#include "my_rsm.h"// TODO file is missing!!!
class MyRsm;

#include <map>

class MyRsw {
protected:
	ro::RSW *rsw;
	ro::GND *gnd;

	/** The ro::GRF object used to read the files from. This pointer is not managed by this class */
	ro::GRF *grf;
	unsigned int gl_list;
	unsigned int *textures;

	typedef std::map<std::string,MyRsm*> mapRsm;
	typedef mapRsm::iterator itrRsm;
	typedef mapRsm::const_iterator citrRsm;
	mapRsm m_rsm;

public:
	MyRsw();

	MyRsw(ro::GRF* grf);
	void setGRF(ro::GRF* grf);
	MyRsw(ro::GRF* grf, const std::string& fn);
	bool registerTexture(std::istream& sdata, const int& texid);
	bool registerTexture(const std::string& fn, const int& texid, const std::string& out = "");
	bool LoadTextures();
	bool LoadModels();
	MyRsm* LoadRSM(const std::string& fn);
	bool Read(const std::string& name);
	void Clear();
	void Draw();
	void DrawModels();
};

#endif /* __MY_RSW_H */
