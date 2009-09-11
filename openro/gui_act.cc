#include "stdafx.h"

#include "gui_act.h"

void GuiAct::DrawAct(const RO::ACT::Pat& pat, sdle::Texture& t) {
	Window((float)pat.spr[0].x, (float)pat.spr[0].y, t, (pat.spr[0].mirrorOn == 1), true);
}
