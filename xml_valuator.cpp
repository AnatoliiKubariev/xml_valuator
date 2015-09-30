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
	if (node_name == "expressions" && valuator->state.nodes.empty())
	{
		valuator->state.nodes.push(node(std::string(name)));
		return;
	}

	node& top_node = valuator->state.nodes.top();
	if (top_node.name == "addition")
	{
		if (node_name != "item")
			return;
	}
	else
	if (top_node.name == "subtraction")
	{
		if (node_name != "minuend" && node_name != "subtrahend")
			return;
	}
	else
	if (top_node.name == "multiplication")
	{
		if (node_name != "factor")
			return;
	}
	else
	if (top_node.name == "division")
	{
		if (node_name != "dividend" && node_name != "divisor")
			return;
	}

	if (std::string(name) == "addition")
		valuator->state.result = 0;
	if (std::string(name) == "multiplication")
		valuator->state.result = 1;

	valuator->state.nodes.push(node(std::string(name)));
}

void xml_valuator::value(void* userData, const char* val, int len)
{
	xml_valuator* valuator = reinterpret_cast<xml_valuator*>(userData);
	node& top_node = valuator->state.nodes.top();

	std::istringstream stream(std::string(val, len));
	double value = 0;
	stream >> value;
	if (stream)
	{
		top_node.value = value;
		calculate(&valuator->state, top_node.name);
	}

}

void xml_valuator::end(void *userData, const char *name)
{
	xml_valuator* valuator = reinterpret_cast<xml_valuator*>(userData);
	node& top_node = valuator->state.nodes.top();
	std::string node_name(name);

	if (node_name == "addition")
	{
		valuator->os << top_node.name << ": " << valuator->state.result << std::endl;
		valuator->state.nodes.pop();
	}
	else
	if (node_name == "subtraction")
	{
		valuator->os << top_node.name << ": " << valuator->state.result << std::endl;
		valuator->state.nodes.pop();
	}
	else
	if (node_name == "multiplication")
	{
		valuator->os << top_node.name << ": " << valuator->state.result << std::endl;
		valuator->state.nodes.pop();
	}
	else
	if (node_name == "division")
	{
		valuator->os << top_node.name << ": " << valuator->state.result << std::endl;
		valuator->state.nodes.pop();
	}
}

void xml_valuator::calculate(xml_state* state, const std::string& name)
{
	node& node = state->nodes.top();
	if (name == "item")
	{
		state->result += node.value;
	}
	else
	if (name == "minuend")
	{
		state->result = node.value;
	}
	else
	if (name == "subtrahend")
	{
		state->result -= node.value;
	}
	else
	if (name == "factor")
	{
		state->result *= node.value;
	}
	else
	if (name == "dividend")
	{
		state->result = node.value;
	}
	else
	if (name == "divisor")
	{
		state->result /= node.value;
	}
	else
	{
		state->nodes.pop();
		return;
	}
}