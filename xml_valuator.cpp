#include "xml_valuator.h"
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

xml_valuator::xml_valuator(std::ostream& os) : os(os)
{
	parser = XML_ParserCreate(nullptr);

	XML_SetElementHandler(parser, start, end);
	XML_SetCharacterDataHandler(parser, value);
	XML_SetUserData(parser, this);

	std::unique_ptr<handler_t> addition(new addition_handler_t());
	handlers.push_back(std::move(addition));
}

xml_valuator::~xml_valuator()
{
	XML_ParserFree(parser);
}

void xml_valuator::calculate(const std::string& fname)
{
	std::vector<char> part(1024);
	std::ifstream file(fname);
	while (file)
	{
		file.read(&part[0], part.size());
		if (XML_Parse(parser, &part[0], file.gcount(), file.gcount() < part.size()) == 0)
		{
			const int code = XML_GetErrorCode(parser);
			const std::string msg(XML_ErrorString((XML_Error)code));
			os << "Parsing error code" << code << " " << msg << std::endl;
			break;
		}
	}
}

void xml_valuator::start(void* userData, const char* name, const char* args[])
{
	xml_valuator* valuator = reinterpret_cast<xml_valuator*>(userData);
	std::string node_name(name);

	if (node_name == "expressions" && valuator->nodes.empty())
	{
		valuator->nodes.push(node_name);
		return;
	}

	for (auto& h : valuator->handlers)
	{
		h->start(node_name, valuator->nodes);
	}

	valuator->nodes.push(node_name);
}

void xml_valuator::value(void* userData, const char* val, int len)
{
	xml_valuator* valuator = reinterpret_cast<xml_valuator*>(userData);

	std::istringstream stream(std::string(val, len));
	double value = 0;
	stream >> value;
	if (stream)
	{
		for (auto& h : valuator->handlers)
		{
			h->value(valuator->nodes, value);
		}
	}

}

void xml_valuator::end(void *userData, const char *name)
{
	xml_valuator* valuator = reinterpret_cast<xml_valuator*>(userData);

	for (auto& h : valuator->handlers)
	{
		h->end(std::string(name), valuator->os);
	}

	valuator->nodes.pop();
}