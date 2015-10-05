#include "xml_valuator.h"
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>

xml_valuator_t::xml_valuator_t(std::ostream& os) : os(os) {}

void xml_valuator_t::open_tag(const std::string& name)
{
    if (!handler && name == "addition")
        handler.reset(new addition_handler_t(handler_t::root));
    if (!handler && name == "multiplication")
        handler.reset(new multiplication_handler_t(handler_t::root));
    if (!handler && name == "subtraction")
        handler.reset(new subtraction_handler_t(handler_t::root));
    if (!handler && name == "division")
        handler.reset(new division_handler_t(handler_t::root));


    if (handler)
        handler->open_tag(name, nodes.empty() ? "" : nodes.top());
}
void xml_valuator_t::value(const std::string& value)
{
    std::istringstream stream(value);
    double val = 0;
    stream >> val;
    if (!stream)
        return;

    if (handler)
        handler->value(nodes.top(), val);
}
void xml_valuator_t::close_tag(const std::string& name)
{

    if (handler && handler->close_tag(name, os) == handler_t::complete)
        handler.reset();
}