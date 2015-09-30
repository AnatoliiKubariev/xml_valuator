#pragma once

#include <string>
#include <stack>

class node
{
public:
	node(std::string name) : name(name){};

	std::string name;
	double value;
};

class xml_state
{
public:
	//xml_state();
	void clear();

	std::stack<node> nodes;
	double result;
};
