#pragma once

#include <string>
#include <stack>
#include <vector>
#include <memory>

class handler_t
{
public:
	virtual ~handler_t() = 0 {};

    enum status_t{ incomplete, complete };
    enum role_t{ child, root };

    double get_result();

    virtual void open_tag(const std::string& name, const std::string& parent_node){};
	virtual void value(const std::string& parent_node, const double value){};
    virtual status_t close_tag(const std::string& name, std::ostream& os){ return incomplete; };
protected:
	double result;
    role_t role;
};

class addition_handler_t : public handler_t
{
public:
    addition_handler_t(role_t role);
private:
	void open_tag(const std::string& name, const std::string& parent_node);
	void value(const std::string& name, const double value);
    status_t close_tag(const std::string& name, std::ostream& os);

	std::unique_ptr<handler_t> nested_handler;
};

class multiplication_handler_t : public handler_t
{
public:
    multiplication_handler_t(role_t role);
private:
    void open_tag(const std::string& name, const std::string& parent_node);
    void value(const std::string& name, const double value);
    status_t close_tag(const std::string& name, std::ostream& os);

    std::unique_ptr<handler_t> nested_handler;
};

class subtraction_handler_t : public handler_t
{
public:
    subtraction_handler_t(role_t role);
private:
    void open_tag(const std::string& name, const std::string& parent_node);
    void value(const std::string& name, const double value);
    status_t close_tag(const std::string& name, std::ostream& os);

    std::unique_ptr<handler_t> nested_handler;
    double minuend;
    double subtrahend;
};

class division_handler_t : public handler_t
{
public:
    division_handler_t(role_t role);
private:
    void open_tag(const std::string& name, const std::string& parent_node);
    void value(const std::string& name, const double value);
    status_t close_tag(const std::string& name, std::ostream& os);

    std::unique_ptr<handler_t> nested_handler;
    double dividend;
    double divisor;
};