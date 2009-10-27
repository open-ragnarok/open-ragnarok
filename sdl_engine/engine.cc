/*
 * engine.cc
 *
 *  Created on: 14/07/2009
 *      Author: N0C2
 */
#include "stdafx.h"
#include "sdle/engine.h"

namespace sdle {
namespace gui {

Engine::Engine() {
	// TODO Auto-generated constructor stub

}

Engine::~Engine() {
	// TODO Auto-generated destructor stub
}

bool Engine::Add(const std::string & s, Frame *f) {
	std::map<std::string, Frame*>::iterator itr;
	itr = m_frames.find(s);
	if (itr != m_frames.end())
		return (false);

	m_frames[s] = f;
	return (true);
}

bool Engine::Remove(const std::string & s) {
	std::map<std::string, Frame*>::iterator itr;
	itr = m_frames.find(s);
	if (itr == m_frames.end())
		return (false);

	m_frames.erase(itr);
	return (true);
}

void Engine::Draw() {
	std::map<std::string, Frame*>::iterator itr;
	itr = m_frames.begin();
	while (itr != m_frames.end()) {
		if (itr->second->isVisible()) {
			itr->second->Draw();
		}

		itr++;
	}
}

int Engine::Count() const {
	return(m_frames.size());
}

}
}
