#ifndef __GUI_ACT_H
#define __GUI_ACT_H

#include "roengine/objects/act_object.h"

class GuiAct : public ActObject {
protected:
	virtual void DrawAct(const RO::ACT::Pat& pat, sdle::Texture& t);
};

#endif /* __GUI_ACT_H */
