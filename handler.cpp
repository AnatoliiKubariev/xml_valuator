#include "handler.h"

double handler_t::get_result()
{
    return result;
}

//addition
addition_handler_t::addition_handler_t(handler_t::role_t role)
{
    this->role = role;
    result = 0;
}

void addition_handler_t::open_tag(const std::string& name, const std::string& parent_node)
{
    if (name == "addition" && parent_node == "item")
    {
        nested_handler.reset(new addition_handler_t(child));
        return;
    }
    if (name == "multiplication" && parent_node == "item")
    {
        nested_handler.reset(new multiplication_handler_t(child));
        return;
    }
    if (nested_handler)
        nested_handler->open_tag(name, parent_node);
}
void addition_handler_t::value(const std::string& parent_node, const double value)
{
    if (nested_handler)
        nested_handler->value(parent_node, value);
    else
    if (parent_node == "item")
        result += value;
}
handler_t::status_t addition_handler_t::close_tag(const std::string& name, std::ostream& os)
{
    status_t nested_end_status = incomplete;
    if (nested_handler)
    {
        nested_end_status = nested_handler->close_tag(name, os);
    }

    if (nested_end_status == complete)
    {
        result += nested_handler->get_result();
        nested_handler.reset();
        return incomplete;
    }

    if (name != "addition")
        return incomplete;

    if (!nested_handler && role == root)
        os << name << ": " << result << std::endl;

    return complete;
}

//multiplication
multiplication_handler_t::multiplication_handler_t(handler_t::role_t role)
{
    this->role = role;
    result = 1;
}

void multiplication_handler_t::open_tag(const std::string& name, const std::string& parent_node)
{
    if (name == "addition" && parent_node == "factor")
    {
        nested_handler.reset(new addition_handler_t(child));
        return;
    }
    if (name == "multiplication" && parent_node == "factor")
    {
        nested_handler.reset(new multiplication_handler_t(child));
        return;
    }
    if (nested_handler)
        nested_handler->open_tag(name, parent_node);
}
void multiplication_handler_t::value(const std::string& parent_node, const double value)
{
    if (nested_handler)
        nested_handler->value(parent_node, value);
    else
    if (parent_node == "factor")
        result *= value;
}
handler_t::status_t multiplication_handler_t::close_tag(const std::string& name, std::ostream& os)
{
    status_t nested_end_status = incomplete;
    if (nested_handler)
    {
        nested_end_status = nested_handler->close_tag(name, os);
    }

    if (nested_end_status == complete)
    {
        result *= nested_handler->get_result();
        nested_handler.reset();
        return incomplete;
    }

    if (name != "multiplication")
        return incomplete;

    if (!nested_handler && role == root)
        os << name << ": " << result << std::endl;

    return complete;
}