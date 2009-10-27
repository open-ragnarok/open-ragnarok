#include "stdafx.h"

#include "sdle/sdle_string.h"

#include <string.h>

namespace sdle {

String::String() {
	m_data = NULL;
	m_cstr = NULL;
	m_length = 0;
	m_buflen = 0;
}

String::String(const char* c) {
	m_cstr = NULL;
	m_buflen = 16;
	m_length = strlen(c);
	while(m_buflen < (m_length+1))
		m_buflen <<= 1;

	if (m_length == 0) {
		m_buflen = 0;
		m_data = NULL;
	}
	else {
		m_data = new unsigned short[m_buflen];
		for (unsigned int i = 0; i < m_length; i++) {
			m_data[i] = c[i];
			m_data[i+1] = 0;
		}
	}
}

String::String(const unsigned short* c) {
	m_cstr = NULL;
	m_buflen = 16;
	m_length = 0;

	while (c[m_length] != 0)
		m_length++;

	while(m_buflen < (m_length+1))
		m_buflen <<= 1;

	if (m_length == 0) {
		m_buflen = 0;
		m_data = NULL;
	}
	else {
		m_data = new unsigned short[m_buflen];
		for (unsigned int i = 0; i < m_length; i++) {
			m_data[i] = c[i];
			m_data[i+1] = 0;
		}
	}
}

String::String(const String& s) {
	m_data = NULL;
	m_cstr = NULL;
	m_buflen = 0;

	m_length = s.m_length;
	if (m_length > 0) {
		m_buflen = 16;
		while(m_buflen < (m_length+1))
			m_buflen <<= 1;

		m_data = new unsigned short[m_buflen];
		for (unsigned int i = 0; i < m_length; i++) {
			m_data[i] = s.m_data[i];
			m_data[i+1] = 0;
		}
	}
}

String::~String() {
	if (m_data != NULL)
		delete[] m_data;
	if (m_cstr != NULL)
		delete[] m_cstr;
}

unsigned int String::len() const {
	return(m_length);
}

const char* String::c_str() {
	if (m_cstr != NULL)
		delete[] m_cstr;

	m_cstr = new char[m_length * 2 + 1];
	char* ptr = m_cstr;
	for (unsigned int i = 0; i < m_length; i++) {
		if (m_data[i] < 256) {
			*ptr = (char)m_data[i];
		}
		else {
			// TODO: Convert to UTF-8
			*ptr = '[';
			ptr++;
			*ptr = ']';
		}
		ptr++;
	}
	*ptr = 0;
	return(m_cstr);
}

String& String::operator = (const String& s) {
	unsigned int neededbuf = 16;
	m_length = s.m_length;
	if (m_cstr != NULL) {
		delete[] m_cstr;
		m_cstr = NULL;
	}

	if (s.m_length == 0)
		return(*this);

	while (neededbuf < (m_length + 1))
		neededbuf <<= 1;

	if (neededbuf != m_buflen) {
		if (m_buflen > 0)
			delete [] m_data;
		m_buflen = neededbuf;
		m_data = new unsigned short[m_buflen];
	}

	for (unsigned int i = 0; i < m_length; i++) {
		m_data[i] = s.m_data[i];
		m_data[i+1] = 0;
	}

	return(*this);
}

String& String::operator = (const unsigned short* s) {
	unsigned int param_len = 0;
	unsigned int neededbuf = 16;

	if (m_cstr != NULL) {
		delete[] m_cstr;
		m_cstr = NULL;
	}

	while (s[param_len] != 0) {
		param_len++;
	}

	m_length = param_len;

	if (m_length == 0)
		return(*this);

	while (neededbuf < (m_length + 1))
		neededbuf <<= 1;

	if (neededbuf != m_buflen) {
		if (m_buflen > 0)
			delete [] m_data;
		m_buflen = neededbuf;
		m_data = new unsigned short[m_buflen];
	}

	for (unsigned int i = 0; i < m_length; i++) {
		m_data[i] = s[i];
		m_data[i+1] = 0;
	}

	return(*this);
}

String& String::operator = (const char* s) {
	unsigned int param_len = 0;
	unsigned int neededbuf = 16;

	if (m_cstr != NULL) {
		delete[] m_cstr;
		m_cstr = NULL;
	}

	while (s[param_len] != 0) {
		param_len++;
	}

	m_length = param_len;

	if (m_length == 0)
		return(*this);

	while (neededbuf < (m_length + 1))
		neededbuf <<= 1;

	if (neededbuf != m_buflen) {
		if (m_buflen > 0)
			delete [] m_data;
		m_buflen = neededbuf;
		m_data = new unsigned short[m_buflen];
	}

	for (unsigned int i = 0; i < m_length; i++) {
		m_data[i] = (unsigned short)s[i];
		m_data[i+1] = 0;
	}

	return(*this);
}


}
