#ifndef __SDLENGINE_SDLE_STRING_H
#define __SDLENGINE_SDLE_STRING_H

namespace sdle {

/** String class, compatible with UTF-16
 */
class String {
protected:
	unsigned short* m_data;
	char* m_cstr;
	unsigned int m_length;
	unsigned int m_buflen;

public:
	String();
	String(const char*);
	String(const unsigned short*);
	String(const String&);
	~String();

	unsigned int len() const;
	const char* c_str();

	String& operator = (const String&);
	String& operator = (const unsigned short*);
	String& operator = (const char*);
};

}

#endif /* __SDLENGINE_SDLE_STRING_H */
