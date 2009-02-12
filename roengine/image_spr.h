#ifndef __IMAGE_SPR_H
#define __IMAGE_SPR_H

#include "image.h"

#include "spr.h"

class ImageSPR : public Image {
protected:
	unsigned int last_idx;
public:
	ImageSPR(const RO::SPR*, const unsigned int& idx);
};


#endif /* __IMAGE_SPR_H */