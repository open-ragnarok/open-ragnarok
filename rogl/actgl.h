/* $Id$ */
#ifndef __ACTGL_H
#define __ACTGL_H

#include "globject.h"

#include "act.h"
#include "spr.h"
#include "grf.h"

#include <string>

namespace ROGL {

class ActData;

class MYLIB_DLLAPI ActGL : public GLObject {
protected:
	ActGL* ext;
	const ActData* act;
public:
	ActGL(const ActData*);
	ActGL(const std::string& name);
	~ActGL();
};

class MYLIB_DLLAPI ActData {
protected:
	RO::ACT *act;
	RO::SPR *spr;
//	RO::PAL *pal;

	std::string m_name;
public:
	ActData();
	ActData(const ActData&);
	virtual ~ActData();

	ActData& operator = (const ActData&);

	void Draw() const;

	bool readAct(std::istream&);
	bool readSpr(std::istream&);

	bool read(const std::string& name, RO::GRF&);

	void setName(const std::string&);
	const std::string& getName() const;
};

}

#endif /* __ACT_H */