#include "stdafx.h"

#include "gui_act.h"

void GuiAct::DrawAct(const ro::ACT::Motion& mot, sdle::Texture& t) {
	Window((float)mot.getClip(0).x, (float)mot.getClip(0).y, t, (mot.getClip(0).mirrorOn == 1), true);
}
