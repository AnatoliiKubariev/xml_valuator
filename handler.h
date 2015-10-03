#pragma once

#include <string>
#include <stack>
#include <vector>
#include <memory>

class handler_t
{
public:
	virtual ~handler_t() = 0 {};

	virtual void start(const std::string& name, const std::string& parent_node){};
	virtual void value(const std::string& parent_node, const double value){};
	virtual void end(const std::string& name, std::ostream& os){};
protected:
	double result;
	bool value_flag;
	bool end_flag;
};

class addition_handler_t : public handler_t
{
public:
	addition_handler_t();
private:
	void start(const std::string& name, const std::string& parent_node);
	void value(const std::string& parent_node, const double value);
	void end(const std::string& name, std::ostream& os);

	std::vector<std::unique_ptr<handler_t>> handlers;
};
