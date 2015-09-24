#include <iostream>
#include <string>
#include <iomanip>

#include "thirdparty\pugixml\src\pugixml.hpp"

double calculate(const pugi::xml_node& node);
double get_component(const pugi::xml_node& node, const bool is_complex);

double addition(const pugi::xml_node& node)
{
	if (std::string(node.name()) != "addition")
		return 0x5D0;

	double result = 0;
	const bool is_complex = node.attribute("complex").as_bool();
	for (pugi::xml_node temp_node = node.child("item"); temp_node; temp_node = temp_node.next_sibling("item"))
	{
		result += get_component(temp_node, is_complex);
	}
	return result;
}

double subtraction(const pugi::xml_node& node)
{
	if (std::string(node.name()) != "subtraction")
		return 0x5D0;

	const bool is_complex = node.attribute("complex").as_bool();
	const double complex = get_component(node.child("minuend"), is_complex);
	const double subtrahend = get_component(node.child("subtrahend"), is_complex);
	return complex - subtrahend;
}

double multiplication(const pugi::xml_node& node)
{
	if (std::string(node.name()) != "multiplication")
		return 0x5D0;

	double result = 1;
	const bool is_complex = node.attribute("complex").as_bool();
	for (pugi::xml_node temp_node = node.child("factor"); temp_node; temp_node = temp_node.next_sibling("factor"))
	{
		result *= get_component(temp_node, is_complex);
	}
	return result;
}

double division(const pugi::xml_node& node)
{
	if (std::string(node.name()) != "division")
		return 0x5D0;

	const bool is_complex = node.attribute("complex").as_bool();
	const double dividend = get_component(node.child("dividend"), is_complex);
	const double divisor = get_component(node.child("divisor"), is_complex);
	return dividend / divisor;
}


double get_component(const pugi::xml_node& node, const bool is_complex)
{
	if (is_complex && node.first_child().first_child())
		return calculate(node.first_child());
	else
		return node.text().as_double();
}

double calculate(const pugi::xml_node& node)
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

	std::cout << std::boolalpha;
	for (pugi::xml_node node = doc.child("expressions").first_child(); node; node = node.next_sibling())
	{
		//Test
		switch (node.attribute("id").as_int())
		{
		case 10:
			std::cout << node.name() << "\n";
			std::cout << (calculate(node) == 9) << "\n";
			break;
		case 11:
			std::cout << node.name() << "\n";
			std::cout << (calculate(node) == 1) << "\n";
			break;
		case 12:
			std::cout << node.name() << "\n";
			std::cout << (calculate(node) == 240) << "\n";
			break;
		case 13:
			std::cout << node.name() << "\n";
			std::cout << (calculate(node) == 1814400) << "\n";
			break;
		case 14:
			std::cout << node.name() << "\n";
			std::cout << (calculate(node) == 6) << "\n";
			break;
		default:
			return -1;
		}
	}

	std::cin.get();
	return 0;
}