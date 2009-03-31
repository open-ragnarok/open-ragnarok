#ifndef __ROGL_IMAGE_SPR_H
#define __ROGL_IMAGE_SPR_H

#include "image.h"

#include "ro/types/spr.h"

namespace rogl {
	class ROGL_DLLAPI ImageSPR : public Image {
	protected:
		unsigned int last_idx;
	public:
		ImageSPR(const RO::SPR*, const unsigned int& idx);
	};
}

#endif /* __ROGL_IMAGE_SPR_H */