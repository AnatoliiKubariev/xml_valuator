#include "xml_valuator.h"
#include <sstream>
#include <string>
#include <fstream>
#include <vector>

xml_valuator_t::xml_valuator_t(std::ostream& os) : os(os) {}

void xml_valuator_t::open_tag(const std::string& name)
{
    if (!handler)
    {
        handler = create_handler(name);
        return;
    }
    if (handler)
        handler->open_tag(name);
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

    if (handler && handler->close_tag(name) == handler_t::complete)
    {
        os << name << ": " << handler->get_result() << std::endl;
        handler.reset();
    }
}
