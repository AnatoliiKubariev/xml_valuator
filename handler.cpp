#include "handler.h"

//-----handler-----
handler_t::handler_t(const double result)
: result(result){}

double handler_t::get_result()
{
    return result;
}

void handler_t::open_tag(const std::string& name)
{
    if (!nested_handler)
    {
        nested_handler = create_handler(name);
        return;
    }
    else
    if (nested_handler)
        nested_handler->open_tag(name);
}
bool handler_t::value(const std::string& name, const double value)
{
    if (nested_handler)
    {
        nested_handler->value(name, value);
        return true;
    }
    return false;
}
handler_t::status_t handler_t::close_tag(const std::string& name)
{
    if (nested_handler)
    {
        return nested_handler->close_tag(name);
    }
    else
        return incomplete;
}

//-----addition-----
addition_handler_t::addition_handler_t()
: handler_t(0){}

bool addition_handler_t::value(const std::string& name, const double value)
{
    if (handler_t::value(name, value))
        return true;

    if (name == "item")
    {
        result += value;
        return true;
    }
    return false;
}
handler_t::status_t addition_handler_t::close_tag(const std::string& name)
{
    if (handler_t::close_tag(name) == complete)
    {
        result += nested_handler->get_result();
        nested_handler.reset();
        return incomplete;
    }

    if (name != "addition")
        return incomplete;

    return complete;
}

//-----multiplication-----
multiplication_handler_t::multiplication_handler_t()
: handler_t(1){}

bool multiplication_handler_t::value(const std::string& name, const double value)
{
    if (handler_t::value(name, value))
        return true;

    if (name == "factor")
    {
        result *= value;
        return true;
    }
    return false;
}
handler_t::status_t multiplication_handler_t::close_tag(const std::string& name)
{
    if (handler_t::close_tag(name) == complete)
    {
        result *= nested_handler->get_result();
        nested_handler.reset();
        return incomplete;
    }
    if (name != "multiplication")
        return incomplete;

    return complete;
}

//------subtraction------
subtraction_handler_t::subtraction_handler_t()
: handler_t(1)
, minuend(0)
, subtrahend(0){}

bool subtraction_handler_t::value(const std::string& name, const double value)
{
    if (handler_t::value(name, value))
        return true;

    if (!nested_handler)
    if (name == "subtrahend")
    {
        subtrahend = value;
        return true;
    }
    if (name == "minuend")
    {
        minuend = value;
        return true;
    }
    return false;
}
handler_t::status_t subtraction_handler_t::close_tag(const std::string& name)
{
    if (handler_t::close_tag(name) == complete)
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

    if (!nested_handler)
    {
        result = minuend - subtrahend;
    }

    return complete;
}

//-----division-----
division_handler_t::division_handler_t()
: handler_t(0)
, dividend(0)
, divisor(0) {}

bool division_handler_t::value(const std::string& name, const double value)
{
    if (handler_t::value(name, value))
        return true;

    if (name == "dividend")
    {
        dividend = value;
        return true;
    }
    if (name == "divisor")
    {
        divisor = value;
        return true;
    }
    return false;
}
handler_t::status_t division_handler_t::close_tag(const std::string& name)
{
    status_t nested_end_status = handler_t::close_tag(name);

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

    if (!nested_handler)
        result = dividend / divisor;

    return complete;
}

std::unique_ptr<handler_t> create_handler(const std::string& name)
{
    if (name == "addition")
    {
        return std::make_unique<addition_handler_t>();
    }
    if (name == "subtraction")
    {
        return std::make_unique<subtraction_handler_t>();
    }
    if (name == "multiplication")
    {
        return std::make_unique<multiplication_handler_t>();
    }
    if (name == "division")
    {
        return std::make_unique<division_handler_t>();
    }
    return std::unique_ptr<handler_t>(nullptr);
}