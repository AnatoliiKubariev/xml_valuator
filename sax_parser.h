#pragma once

#include <expat.h>
#include <stack>

class sax_parser_t
{
public:
	sax_parser_t();
	virtual ~sax_parser_t() = 0;

	void parse(std::istream& is);
protected:
	std::stack<std::string> nodes;
private:
	virtual void open_tag(const std::string& name){};
	virtual void value(const std::string& value){};
	virtual void close_tag(const std::string& name){};

	static void start(void* userData, const char* name, const char* args[]);
	static void value(void* userData, const char* val, int len);
	static void end(void *userData, const char *name);

	XML_Parser parser;
};