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

class LogSys {
private:
	static LogSys* m_singleton;

	std::map<int, std::string> m_prefixes;
	std::map<int, FILE*> m_outputs;

public:
	LogSys();
	~LogSys();

	LogSys& log(const char*);

	LogSys& addHandler(int identifier, const std::string& prefix = "");
	LogSys& setOutput(int identifier, FILE* out);
	LogSys& log(int identifier, const char*, ...);
	LogSys& hexlog(int identifier, const unsigned char* data, unsigned int datalen);

	static LogSys* GetSingleton();
};

#endif /* LOGSYS_H_ */
