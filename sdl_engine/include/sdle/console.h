#ifndef __SDLENGINE_CONSOLE_H
#define __SDLENGINE_CONSOLE_H

#include "sdle_settings.h"

#ifdef SDLENGINE_CONSOLE

#include "font.h"
#include <vector>
#include <string>

#include <SDL_keysym.h>
#include <SDL_events.h>

namespace sdle {

class Console {
protected:
	/** The key needed to toggle the console. Default = "'" (quote) */
	unsigned int m_activate_key;

	/** The modifier needed to toggle the console. Default = Left Alt */
	unsigned int m_activate_mod;
	bool m_active;

	/** Number of lines displayed by the console */
	unsigned int m_linecount;

	/** Font used by the console */
	Font* m_font;

	bool m_activated;

	std::vector<std::string> m_lines;
	std::string m_commandline;
public:
	Console();

	~Console();

	/**
	 * Sets the font used by the console
	 * @param f the Font object to use
	 */
	bool setFont(Font* f);

	/**
	 * Sets the visibility of the console
	 */
	void setVisible(bool);

	/**
	 * Injects event. If not handled by the console, returns 'false'.
	 */
	bool InjectKeyDown(const SDL_Event* e);
	bool InjectKeyUp(const SDL_Event* e);

	void add(const std::string&);

	void Draw();

};

}
#endif /* SDLENGINE_CONSOLE */

#endif /* __SDLENGINE_CONSOLE_H */

