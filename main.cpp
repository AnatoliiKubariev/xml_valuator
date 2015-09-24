#include <iostream>
#include <string>

#include "thirdparty\pugixml\src\pugixml.hpp"

double calculate(const pugi::xml_node node);

double addition(const pugi::xml_node node)
{
	if (std::string(node.name()) != "addition")
		return 0x5D0;

	double result = 0;

	for (pugi::xml_node temp_node = node.child("item"); temp_node; temp_node = temp_node.next_sibling("item"))
	{
		if (temp_node.first_child().first_child())
			result += calculate(temp_node.first_child());
		else
		result += temp_node.text().as_double();
	}
	return result;
}

double subtraction(const pugi::xml_node node)
{
	if (std::string(node.name()) != "subtraction")
		return 0x5D0;

	double result = node.child("minuend").text().as_double();
	result -= node.child("subtrahend").text().as_double();
	return result;
}

double multiplication(const pugi::xml_node node)
{
	if (std::string(node.name()) != "multiplication")
		return 0x5D0;

	double result = 1;

	for (pugi::xml_node temp_node = node.child("factor"); temp_node; temp_node = temp_node.next_sibling("factor"))
	{
		if (temp_node.first_child().first_child())
			result *= calculate(temp_node.first_child());
		else
		result *= temp_node.text().as_double();
	}
	return result;
}

double division(const pugi::xml_node node)
{
	if (std::string(node.name()) != "division")
		return 0x5D0;

	double result = 0;
	if (node.child("dividend").first_child().first_child())
		result = calculate(node.child("dividend").first_child());
	else
	result = node.child("dividend").text().as_double();

	if (node.child("divisor").first_child().first_child())
		result /= calculate(node.child("divisor").first_child());
	else
	result /= node.child("divisor").text().as_double();

	return result;
}

double calculate(const pugi::xml_node node)
{
	if (std::string(node.name()) == "addition")
		return addition(node);
	if (std::string(node.name()) == "subtraction")
		return subtraction(node);
	if (std::string(node.name()) == "multiplication")
		return multiplication(node);
	if (std::string(node.name()) == "division")
		return division(node);
}


int main()
{
	pugi::xml_document doc;
	if (!doc.load_file("data0002.xml"))
		return -1;

	for (pugi::xml_node node = doc.child("expressions").first_child(); node; node = node.next_sibling())
	{
		std::cout << node.name() << "\n";
		std::cout << calculate(node) << "\n";
	}
		std::cin.get();
		return 0;
	}