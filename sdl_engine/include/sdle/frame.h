/*
 * frame.h
 *
 *  Created on: 14/07/2009
 *      Author: N0C2
 */

#ifndef FRAME_H_
#define FRAME_H_

#include "sdle_import.h"

namespace sdle {
namespace gui {

/** Base visual object of the gui */
class SDLENGINE_DLLAPI Frame {
protected:
	int x, y;
	int w, h;
	bool m_visible;
public:
	Frame();
	virtual ~Frame();

	bool isVisible() const;
	void setVisible(bool);

	void setPosition(int, int);
	void setDimension(int, int);
	int getX() const;
	int getY() const;
	int getWidth() const;
	int getHeight() const;

	virtual void Draw();
};

}
}

#endif /* FRAME_H_ */
