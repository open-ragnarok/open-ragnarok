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
#ifdef _MSC_VER
#include <windows.h>
#endif

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
	printf("%s", data);
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
#ifdef _MSC_VER
			HANDLE hStdout;
			WORD wAttributes;
			CONSOLE_SCREEN_BUFFER_INFO csbi;

			if (out == stdout)
				hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
			else if (out == stderr)
				hStdout = GetStdHandle(STD_ERROR_HANDLE);
			GetConsoleScreenBufferInfo(hStdout, &csbi);

			switch(identifier & 0x07) {
			case 1: // Debug
				wAttributes = FOREGROUND_GREEN;
				break;
			case 2: // Error
				wAttributes = FOREGROUND_RED;
				break;
			case 3: // Trace
				wAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; // White
				break;
			case 4: // Warning
				wAttributes = FOREGROUND_RED | FOREGROUND_GREEN;
				break;
			default:
				wAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; // White
				break;
			}
			wAttributes = wAttributes | FOREGROUND_INTENSITY;

			SetConsoleTextAttribute(hStdout, wAttributes);
			fprintf(out, "[%s] ", m_prefixes[identifier].c_str());
			SetConsoleTextAttribute(hStdout, csbi.wAttributes);

			fprintf(out, "%s\n", msg);
#else
			fprintf(out, "[%s%s%s] %s\n", CL_BOLD, m_prefixes[identifier].c_str(), CL_RESET, msg);
#endif
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

	log(identifier, "Hex dump %d byte(s)", buflen);
//	fprintf(out, "---------------------------------------------------------------------------\n");
	fprintf(out, ".... | +0 +1 +2 +3 +4 +5 +6 +7  +8 +9 +A +B +C +D +E +F | 01234567 89ABCDEF\n");
//	fprintf(out, "-----|--------------------------------------------------|------------------\n");
//	fprintf(out, "---- | -- -- -- -- -- -- -- --  -- -- -- -- -- -- -- -- | -------- --------\n");

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
			fprintf(out, "%04x | ", pos);
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
//	fprintf(out, "---------------------------------------------------------------------------\n");
	fflush(out);
	return(*this);
}
