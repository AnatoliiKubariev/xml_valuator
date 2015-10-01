#pragma once

#include <ostream>
#include <stack>
#include <string>
#include <memory>
#include <vector>
#include <expat.h>
#include "handler.h"

class xml_valuator
{
public:
	xml_valuator(std::ostream& os);
	~xml_valuator();

	void calculate(const std::string& fname);

private:
	static void start(void* userData, const char* name, const char* args[]);
	static void value(void* userData, const char* val, int len);
	static void end(void *userData, const char *name);

	XML_Parser parser;
	std::stack<std::string> nodes;
	std::vector<std::unique_ptr<handler_t>> handlers;
	std::ostream& os;
};
