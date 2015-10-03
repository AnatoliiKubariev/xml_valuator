#include "handler.h"

addition_handler_t::addition_handler_t()
{
	result = 0;
	value_flag = true;
	end_flag = true;
}

void addition_handler_t::start(const std::string& name, const std::string& parent_node)
{
	if (name == "addition" && parent_node == "item")
	{
		std::unique_ptr<handler_t> addition(new addition_handler_t());
		handlers.push_back(std::move(addition));
		return;
	}

	for (auto& h : handlers)
	{
		h->start(name, parent_node);
	}
}
void addition_handler_t::value(const std::string& parent_node, const double value)
{
	for (auto& h : handlers)
	{
		h->value(parent_node, value);
		value_flag == false;
	}

	if (parent_node == "item" && value_flag == true)
	{
		result += value;
	}
	value_flag == true;
}
void addition_handler_t::end(const std::string& name, std::ostream& os)
{
	for (auto& h : handlers)
	{
		h->end(name, os);
		end_flag = false;
	}

	if (name != "addition" || end_flag == false)
	{
		end_flag = true;
		return;
	}

	value_flag = false;
	end_flag = false;
	os << name << ": " << result << std::endl;
}