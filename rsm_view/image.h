/* $Id: image.h 4 2008-03-01 21:10:32Z sergio $ */
/*
 Open Ragnarok Project 

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2 of the License.
  
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY except by those people which sell it, which
 are required to give you total support for your newly bought product;
 without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
	
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __IMAGE_H
#define __IMAGE_H

#include "blob.h"

/**
 * This class is an class that holds the generic info for a image.
 * Image loads should be implemented in classes derived from this one.
 */
class Image : public DynamicBlob<char> {
protected:
	Image(char*, int w, int h, int bpp = 24);
	
	int width, height, bpp;
public:
	virtual ~Image();
	
	int glDrawType;
	
	int getWidth() const;
	int getHeight() const;
	int getBpp() const;
};

#endif /*IMAGE_H_*/
