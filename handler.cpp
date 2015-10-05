#include "handler.h"

double handler_t::get_result()
{
    return result;
}

//-----addition-----
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
void addition_handler_t::value(const std::string& name, const double value)
{
    if (nested_handler)
        nested_handler->value(name, value);
    else
    if (name == "item")
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

//-----multiplication-----
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
void multiplication_handler_t::value(const std::string& name, const double value)
{
    if (nested_handler)
        nested_handler->value(name, value);
    else
    if (name == "factor")
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

//------subtraction------
subtraction_handler_t::subtraction_handler_t(handler_t::role_t role) : minuend(0), subtrahend(0)
{
    this->role = role;
    result = 0;
}

void subtraction_handler_t::open_tag(const std::string& name, const std::string& parent_node)
{
    if (name == "addition" && parent_node == "minuend")
    {
        nested_handler.reset(new addition_handler_t(child));
        return;
    }
    if (name == "multiplication" && parent_node == "minuend")
    {
        nested_handler.reset(new multiplication_handler_t(child));
        return;
    }
    if (name == "addition" && parent_node == "subtrahend")
    {
        nested_handler.reset(new addition_handler_t(child));
        return;
    }
    if (name == "multiplication" && parent_node == "minuend")
    {
        nested_handler.reset(new multiplication_handler_t(child));
        return;
    }


    if (nested_handler)
        nested_handler->open_tag(name, parent_node);
}
void subtraction_handler_t::value(const std::string& name, const double value)
{
    if (nested_handler)
        nested_handler->value(name, value);
    else
    if (name == "subtrahend")
        subtrahend = value;
    else
    if (name == "minuend")
        minuend = value;
}
handler_t::status_t subtraction_handler_t::close_tag(const std::string& name, std::ostream& os)
{
    status_t nested_end_status = incomplete;
    if (nested_handler)
    {
        nested_end_status = nested_handler->close_tag(name, os);
    }

    if (nested_end_status == complete)
    {
        result = nested_handler->get_result();

        nested_handler.reset();
        return incomplete;
    }

    if (name == "minuend" && result)
    {
        minuend = result;
        result = 0;
    }
    if (name == "subtrahend" && result)
    {
        subtrahend = result;
        result = 0;
    }
    if (name != "subtraction")
        return incomplete;

    if (!nested_handler && role == root)
    {
        result = minuend - subtrahend;
        os << name << ": " << result << std::endl;
    }

    return complete;
}

//-----division-----
division_handler_t::division_handler_t(handler_t::role_t role) : dividend(0), divisor(0)
{
    this->role = role;
    result = 0;
}

void division_handler_t::open_tag(const std::string& name, const std::string& parent_node)
{
    if (name == "addition" && parent_node == "dividend")
    {
        nested_handler.reset(new addition_handler_t(child));
        return;
    }
    if (name == "multiplication" && parent_node == "divisor")
    {
        nested_handler.reset(new multiplication_handler_t(child));
        return;
    }
    if (name == "addition" && parent_node == "dividend")
    {
        nested_handler.reset(new addition_handler_t(child));
        return;
    }
    if (name == "multiplication" && parent_node == "divisor")
    {
        nested_handler.reset(new multiplication_handler_t(child));
        return;
    }


    if (nested_handler)
        nested_handler->open_tag(name, parent_node);
}
void division_handler_t::value(const std::string& name, const double value)
{
    if (nested_handler)
        nested_handler->value(name, value);
    else
    if (name == "dividend")
        dividend = value;
    else
    if (name == "divisor")
        divisor = value;
}
handler_t::status_t division_handler_t::close_tag(const std::string& name, std::ostream& os)
{
    status_t nested_end_status = incomplete;
    if (nested_handler)
    {
        nested_end_status = nested_handler->close_tag(name, os);
    }

    if (nested_end_status == complete)
    {
        result = nested_handler->get_result();

        nested_handler.reset();
        return incomplete;
    }

    if (name == "dividend" && result)
    {
        dividend = result;
        result = 0;
    }
    if (name == "divisor" && result)
    {
        divisor = result;
        result = 0;
    }
    if (name != "division")
        return incomplete;

    if (!nested_handler && role == root)
    {
        result = dividend / divisor;
        os << name << ": " << result << std::endl;
    }

    return complete;
}