#pragma once

#include <expat.h>
#include "xml_state.h"
#include <ostream>

class xml_valuator
{
public:
	xml_valuator(std::ostream& os);

	void calculate(const std::string& fname);

private:
	static void start(void* userData, const char* name, const char* args[]);
	static void value(void* userData, const char* val, int len);
	static void end(void *userData, const char *name);
	static void calculate(xml_state* state, const std::string& name);

	XML_Parser parser;
	xml_state state;
	std::ostream& os;
};