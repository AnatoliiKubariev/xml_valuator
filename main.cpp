#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <expat.h>
#include "xml_state.h"

void calculate(xml_state* state, const std::string& name);

void start(void* userData, const char* name, const char* args[])
{
	xml_state* state = reinterpret_cast<xml_state*>(userData);
	std::string node_name(name);
	if (node_name == "expressions" && state->nodes.empty())
	{
		state->nodes.push(node(std::string(name)));
		return;
	}

	node& top_node = state->nodes.top();
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
		state->result = 0;
	if (std::string(name) == "multiplication")
		state->result = 1;

	state->nodes.push(node(std::string(name)));
}

void value(void* userData, const char* val, int len)
{
	xml_state* state = reinterpret_cast<xml_state*>(userData);
	node& top_node = state->nodes.top();

	std::istringstream stream(std::string(val, len));
	double value = 0;
	stream >> value;
	if (stream)
	{
		top_node.value = value;
		calculate(state, top_node.name);
	}

}

void end(void *userData, const char *name)
{
	xml_state* state = reinterpret_cast<xml_state*>(userData);
	node& top_node = state->nodes.top();
	std::string node_name(name);

	if (node_name == "addition")
	{
		std::cout << top_node.name << ": " << state->result << std::endl;
		state->nodes.pop();
	}
	else
	if (node_name == "subtraction")
	{
		std::cout << top_node.name << ": " << state->result << std::endl;
		state->nodes.pop();
	}
	else
	if (node_name == "multiplication")
	{
		std::cout << top_node.name << ": " << state->result << std::endl;
		state->nodes.pop();
	}
	else
	if (node_name == "division")
	{
		std::cout << top_node.name << ": " << state->result << std::endl;
		state->nodes.pop();
	}
}

void calculate(xml_state* state, const std::string& name)
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

int main()
{
	XML_Parser parser = XML_ParserCreate(nullptr);
	xml_state state;
	XML_SetElementHandler(parser, start, end);

	XML_SetCharacterDataHandler(parser, value);

	XML_SetUserData(parser, &state);


	std::vector<char> part(1024);
	std::ifstream file("data0001.xml");
	while (file)
	{
		file.read(&part[0], part.size());
		if (XML_Parse(parser, &part[0], file.gcount(), file.gcount() < part.size()) == 0)
		{
			const int code = XML_GetErrorCode(parser);
			const std::string msg(XML_ErrorString((XML_Error)code));
			std::cout << "Parsing error code" << code << " " << msg << std::endl;
			break;
		}
	}

	XML_ParserFree(parser);

	std::cout << state.result;
	std::cout << file.tellg();
	std::cin.get();
	return 0;
}