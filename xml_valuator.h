#pragma once

#include "sax_parser.h"
#include "handler.h"
#include <ostream>
#include <string>
#include <memory>

class xml_valuator_t : public sax_parser_t
{
public:
	xml_valuator_t(std::ostream& os);

private:
	void open_tag(const std::string& name);
	void value(const std::string& value);
	void close_tag(const std::string& name);

	std::unique_ptr<handler_t> handler;
	std::ostream& os;
};