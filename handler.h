#pragma once

#include <string>
#include <stack>
#include <vector>
#include <memory>

class handler_t;
std::unique_ptr<handler_t> create_handler(const std::string& name);

class handler_t
{
public:
    enum status_t{ incomplete, complete };

    handler_t(const double result);
    virtual ~handler_t() = 0 {};

    double get_result();

    virtual void open_tag(const std::string& name);
    virtual bool value(const std::string& name, const double value);
    virtual status_t close_tag(const std::string& name);
protected:
    std::unique_ptr<handler_t> nested_handler;
    double result;
};

class addition_handler_t : public handler_t
{
public:
    addition_handler_t();
private:
    bool value(const std::string& name, const double value);
    status_t close_tag(const std::string& name);
};

class multiplication_handler_t : public handler_t
{
public:
    multiplication_handler_t();
private:
    bool value(const std::string& name, const double value);
    status_t close_tag(const std::string& name);
};

class subtraction_handler_t : public handler_t
{
public:
    subtraction_handler_t();
private:
    bool value(const std::string& name, const double value);
    status_t close_tag(const std::string& name);

    double minuend;
    double subtrahend;
};

class division_handler_t : public handler_t
{
public:
    division_handler_t();
private:
    bool value(const std::string& name, const double value);
    status_t close_tag(const std::string& name);

    double dividend;
    double divisor;
};