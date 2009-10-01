/*
 * logsys.h
 *
 *  Created on: 30/09/2009
 *      Author: Sergio Moura
 *    Revision: $Rev$
 */

#ifndef LOGSYS_H_
#define LOGSYS_H_

#include "colors.h"

#include <string>
#include <map>

typedef unsigned int t_logid;

struct LogSysInfo {
	t_logid id;
	const char* prefix;
	bool active;
	FILE* output;
};

class LogSys {
private:
	static LogSys* m_singleton;

	std::map<t_logid, std::string> m_prefixes;
	std::map<t_logid, FILE*> m_outputs;
	std::map<t_logid, bool> m_active;

public:
	LogSys();
	~LogSys();

	LogSys& log(const char*);

	LogSys& addHandler(t_logid identifier, const std::string& prefix = "", bool active = true);
	LogSys& setActive(t_logid identifier, bool active = true);
	LogSys& setOutput(t_logid identifier, FILE* out);
	LogSys& log(t_logid identifier, const char*, ...);
	LogSys& hexlog(t_logid identifier, const unsigned char* data, unsigned int datalen);

	LogSys& registerInfo(struct LogSysInfo*, unsigned int size = 1);

	static LogSys* GetSingleton();
};

#endif /* LOGSYS_H_ */
