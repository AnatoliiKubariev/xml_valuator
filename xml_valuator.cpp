#include "xml_valuator.h"
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>

xml_valuator_t::xml_valuator_t(std::ostream& os) : os(os)
{
	std::unique_ptr<handler_t> addition(new addition_handler_t());
	handlers.push_back(std::move(addition));
}

void xml_valuator_t::open_tag(const std::string& name)
{
	for (auto& h : handlers)
	{
		h->start(name, nodes.empty() ? "" : nodes.top());
	}
}
void xml_valuator_t::value(const std::string& value)
{
	std::istringstream stream(value);
	double val = 0;
	stream >> val;
	if (!stream)
		return;

	for (auto& h : handlers)
	{
		h->value(nodes.top(), val);
	}
}
void xml_valuator_t::close_tag(const std::string& name)
{
	for (auto& h : handlers)
	{
		h->end(name, os);
	}
}