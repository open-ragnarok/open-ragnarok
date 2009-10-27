/* $Id$
 * logsys.cc
 *
 *  Created on: 2009-09-30
 *      Author: Sergio Moura
 *    Revision: $Rev$
 */
#include "stdafx.h"
#include "logsys/logsys.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

LogSys* LogSys::m_singleton = NULL;

LogSys::LogSys() {
	if (m_singleton != NULL) {
		fprintf(stderr, "[WARNING] More than one instance of Logsys created. Unexpected results may happen.\n");
	}
	m_singleton = this;
}

LogSys::~LogSys() {
	m_singleton = NULL;
	printf(CL_RESET);
}

LogSys* LogSys::GetSingleton() {
	if (m_singleton == NULL)
		new LogSys();

	return(m_singleton);
}

LogSys& LogSys::log(const char* data) {
	printf(data);
	printf("\n");
	return(*this);
}

LogSys& LogSys::addHandler(t_logid identifier, const std::string& prefix, bool active) {
	m_prefixes[identifier] = prefix;
	m_active[identifier] = active;

	return(*this);
}

LogSys& LogSys::setOutput(t_logid identifier, FILE* out) {
	m_outputs[identifier] = out;

	return(*this);
}

LogSys& LogSys::setActive(t_logid identifier, bool active) {
	m_active[identifier] = active;

	return(*this);
}


LogSys& LogSys::log(t_logid identifier, const char* fmt, ...) {
	if (!m_active[identifier])
		return(*this);

	va_list args;
	va_start(args, fmt);
	int size = vsnprintf(NULL, 0, fmt, args);
	char * msg = new char[size + 1];
	vsnprintf(msg, size + 1, fmt, args); msg[size] = '\0';

	FILE* out = stdout;
	if (m_outputs.find(identifier) != m_outputs.end())
		out = m_outputs[identifier];

	if (m_prefixes[identifier] != "") {
		if (out == stdout || out == stderr) {
			fprintf(out, "[%s%s%s] %s\n", CL_BOLD, m_prefixes[identifier].c_str(), CL_RESET, msg);
		}
		else {
			fprintf(out, "[%s] %s\n", m_prefixes[identifier].c_str(), msg);
		}
	}
	else {
		fprintf(out, "%s\n", msg);
	}

	delete[] msg;

	return(*this);
}

LogSys& LogSys::registerInfo(struct LogSysInfo* info, unsigned int size) {
	for (unsigned int i = 0; i < size; i++) {
		addHandler(info[i].id, info[i].prefix, info[i].active);
		if (info[i].output != NULL)
			setOutput(info[i].id, info[i].output);
	}

	return(*this);
}


LogSys& LogSys::hexlog(t_logid identifier, const unsigned char* buf, unsigned int buflen) {
	if (!m_active[identifier])
		return(*this);

	FILE* out = stdout;
	if (m_outputs.find(identifier) != m_outputs.end())
		out = m_outputs[identifier];

	char pfx[32];
	if (m_prefixes[identifier] != "") {
		if (out == stdout || out == stderr) {
			sprintf(pfx, "[%s%s%s] ", CL_BOLD, m_prefixes[identifier].c_str(), CL_RESET);
		}
		else {
			sprintf(pfx, "[%s] ", m_prefixes[identifier].c_str());
		}
	}
	else {
		pfx[0] = 0;
	}


	unsigned int pos = 0;
	unsigned int tbufpos = 0;
	char tbuf[32];
	char c;

	tbuf[0] = 0;

	for (pos = 0; pos < buflen; pos++) {
		if (pos % 16 == 0) {
			tbuf[tbufpos] = 0;
			if (pos > 0) {
				fprintf(out, "| %s\n", tbuf);
			}
			fprintf(out, "%s%04x | ", pfx, pos);
			tbufpos = 0;
		}
		else if (pos % 8 == 0) {
			tbuf[tbufpos++] = ' ';
			fprintf(out, " ");
		}
		fprintf(out, "%02x ", buf[pos]);
		c = buf[pos];
		if (c < ' ' || c > 'z')
			c = '.';

		tbuf[tbufpos++] = c;
	}
	int rest = 16 - (pos % 16);
	if (rest < 16) {
		if (rest >= 8)
			fprintf(out, " ");
		for (int i = 0; i < rest; i++)
			fprintf(out, "   ");
	}
	tbuf[tbufpos] = 0;
	fprintf(out, "| %s\n", tbuf);
	fflush(out);
	return(*this);
}
