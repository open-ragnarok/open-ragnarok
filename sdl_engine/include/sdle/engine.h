/* $Id$
 * engine.h
 *
 *  Created on: 2009-07-14
 *      Author: Sergio Moura
 *    Revision: $Rev$
 */

#ifndef __SDLENGINE_ENGINE_H
#define __SDLENGINE_ENGINE_H

#include "sdle_import.h"
#include "sdl_engine.h"
#include "frame.h"

#include <string>
#include <map>

namespace sdle {
namespace gui {

class SDLENGINE_DLLAPI Engine {
protected:
	std::map<std::string, Frame*> m_frames;
public:
	Engine();
	virtual ~Engine();

	virtual void Draw();
	bool Add(const std::string&, Frame*);
	bool Remove(const std::string&);

	int Count() const;
};

}
}

#endif /* __SDLENGINE_ENGINE_H */
