/* $Id$ */
#include "stdafx.h"

#include <iostream>

#include <vector>
#include <map>
#include <string>

#include "basic_objects.h"

class Console {
protected:
	typedef std::map<std::string, Object*> t_var;
	typedef t_var::iterator t_var_iterator;
	typedef t_var::const_iterator t_var_citerator;
	t_var variables;

	typedef std::string(Console::*f_handle)(const std::vector<std::string>&);
	typedef std::map<std::string, f_handle> t_handle;
	typedef t_handle::iterator t_handle_itr;
	typedef t_handle::const_iterator t_handle_citr;
	t_handle handlers;

#define ADD_HANDLER(x) handlers[#x] = &Console::handle_ ##x;

public:
	Console() {
		ADD_HANDLER(del);
		ADD_HANDLER(assign);
		ADD_HANDLER(list);
		ADD_HANDLER(clear);
		// handlers["del"] = &Console::handle_del;
		// handlers["assign"] = &Console::handle_assign;
		// handlers["list"] = &Console::handle_list;
	}

	~Console() {
		Clear();
	}

	void Clear() {
		t_var_iterator itr = variables.begin();
		while (itr != variables.end()) {
			delete(itr->second);
			itr++;
		}

		variables.clear();
	}

	void Set(const std::string& var, Object* data) {
		Del(var);
		variables[var] = data;
	}

	bool Exists(const std::string& var) const {
		t_var_citerator itr = variables.find(var);
		if (itr != variables.end())
			return(true);
		return(false);
	}

	void Del(const std::string& var) {
		t_var_iterator itr = variables.find(var);
		if (itr != variables.end()) {
			delete(itr->second);
			variables.erase(itr);
		}
	}

	void List() const {
		t_var_citerator itr = variables.begin();
		printf("Variable count: %d\n", variables.size());
		while (itr != variables.end()) {
			printf("%s (%s)\n", itr->first.c_str(), itr->second->getObjectType().c_str());
			itr++;
		}
	}

	Object* operator[] (const std::string& var) {
		t_var_iterator itr = variables.find(var);
		if (itr != variables.end()) {
			return(itr->second);
		}
		Object* r = new EmptyObject();
		Set(var, r);
		return(r);
	}

	void Tokenize(const std::string& line, std::vector<std::string>& vector) {
		vector.clear();
		char buf[256];
		unsigned int i;
		memset(buf, 0, 256);
		bool instring = false;
		char stringdelimiter;
		for (i = 0; i < line.length(); i++) {
			if (instring) {
				if (line[i] == stringdelimiter) {
					if (strlen(buf) > 0) {
						vector.push_back(buf);
						memset(buf, 0, 256);
					}
					buf[0] = line[i];
					vector.push_back(buf);
					buf[0] = 0;
					instring = false;
				}
				else {
					buf[strlen(buf)] = line[i];
				}
			}
			else {
				if (line[i] == ' ' || line[i] == '\t' || line[i] == '\r' || line[i] == '\n') {
					if (strlen(buf) > 0) {
						vector.push_back(buf);
						memset(buf, 0, 256);
					}
				}
				else if (line[i] == '=' || line[i] == '.' || line[i] == '(' || line[i] == ')' || line[i] == ',') {
					if (strlen(buf) > 0) {
						vector.push_back(buf);
						memset(buf, 0, 256);
					}
					buf[0] = line[i];
					vector.push_back(buf);
					buf[0] = 0;
				}
				else if (line[i] == '\'' || line[i] == '"') {
					stringdelimiter = line[i];
					instring = true;

					if (strlen(buf) > 0) {
						vector.push_back(buf);
						memset(buf, 0, 256);
					}
					buf[0] = line[i];
					vector.push_back(buf);
					buf[0] = 0;
				}
				else {
					buf[strlen(buf)] = line[i];
				}
			}
		}
		if (strlen(buf) > 0) {
			vector.push_back(buf);
		}

		if (instring) {
			memset(buf, 0, 256);
			buf[0] = stringdelimiter;
			vector.push_back(buf);
		}
	}

	Object* Eval(const std::string& s) {
		// check for number
		if (s[0] >= '0' && s[0] <= '9') {
			// It's a number!
			// Check if it is a float
			if (s.find(".") != std::string::npos) {
				return(new FloatObject(s));
			}
			else {
				return(new IntObject(s));
			}
		}
		else if (s[0] == '"' || s[0] == '\'') {
			std::string aux = s.substr(1, s.length() - 2);
			return(new TextObject(aux));
		}
		return(new EmptyObject());
	}

	std::string ProcessLine(const std::string& line) {
		// [var=]<value|var|[var.]func[([parameter[,parameter[, ...]]])]>
		// parameter is value, var or [var.]func[([parameter...])]

		std::string ret = "";
		std::vector<std::string> tokens;
		Tokenize(line, tokens);

		for (unsigned int i = 0; i < tokens.size(); i++) {
			printf("Token: %s\n", tokens[i].c_str());
		}
		return("");

		if (tokens.size() == 0) // Nothing to do.
			return(ret);

		// Fix things
		if (tokens.size() >= 3 && tokens[1] == "=") {
			std::string p1, p2;
			p1 = tokens[0];
			p2 = tokens[2];
			tokens.clear();
			tokens.push_back("assign");
			tokens.push_back(p1);
			tokens.push_back(p2);
		}

		t_handle_itr itr = handlers.find(tokens[0]);
		if (itr != handlers.end()) {
			tokens.erase(tokens.begin());
			f_handle func = itr->second;
			ret = (this->*func)(tokens);
			return(ret);
		}

		if (tokens.size() == 1) {
			std::string v = tokens[0];
			tokens.clear();
			tokens.push_back("ans");
			tokens.push_back(v);
			ret = handle_assign(tokens);
			return(ret);
		}

		printf("[DEBUG] No handler for %s\n", tokens[0]);

		return(ret);
	}

	std::string handle_del(const std::vector<std::string>& params) {
		std::string ret;
		if (params.size() != 1) {
			std::string ret;
			ret = "Invalid parameter count for 'del': " + params.size();
			return(ret);
		}

		if (!Exists(params[0])) {
			ret = "No such variable " + params[0];
			return(ret);
		}
		Del(params[0]);
		return(ret);
	}

	std::string handle_list(const std::vector<std::string>& params) {
		List();
		return("");
	}

	std::string handle_clear(const std::vector<std::string>& params) {
		Clear();
		return("");
	}

	std::string handle_assign(const std::vector<std::string>& params) {
		std::string ret;
		if (params.size() != 2) {
			std::string ret;
			ret = "Invalid parameter count for 'assign': " + params.size();
			return(ret);
		}

		Object* d = Eval(params[1]);
		Set(params[0], d);
		ret = params[0] + " = " + d->toString();
		return(ret);
	}
};

int main(int argc, char** argv) {
	std::cout << "Open Ragnarok Console" << std::endl;

	Console console;
	console.Set("num1", new IntObject(10));
	console.Set("num2", new FloatObject(10.5));
	console.Set("ans", new IntObject(0));

	char buf[256];
	memset(buf, 0, 256);
	std::string ret;
	do {
		printf("> ");
		gets(buf);
		if (strcmp(buf, "exit") == 0) {
			sprintf(buf, "");
		}
		if (strlen(buf) > 0)
			std::cout << console.ProcessLine(buf) << std::endl;
	} while (strlen(buf) > 0);
	
	return(0); // keep compiler happy
}

// EOF