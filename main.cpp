#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <fstream>

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

void add_addition(std::ostream& os, int id);
void add_subtraction(std::ostream& os, int id);
void add_multiplication(std::ostream& os, int id);
void add_division(std::ostream& os, int id);

void generate_xml(std::ostream& os)
{
	os << "<expressions>" << std::endl;
	//for (int id = 1; id <= 4; ++id)
	int id = 1;
	while (os.tellp() < 1024 * 1024 * 1024)
	{
		int xml_case = rand() % 4 + 1;
		switch (xml_case)
		{
		case 1:
			add_addition(os, id);
			break;
		case 2:
			add_subtraction(os, id);
			break;
		case 3:
			add_multiplication(os, id);
			break;
		case 4:
			add_division(os, id);
			break;
		default:
			break;
		}
		++id;
	}


	os << "</expressions>";
}

void add_addition(std::ostream& os, int id)
{
	os << "<addition id=\"" << id << "\">" << std::endl;

	for (int i = 0; i < rand() % 5 + 2; ++i)
	{
		os << "<item>";
		int item = rand() % 100 + 1;
		os << item;
		os << "</item>" << std::endl;
	}

	os << "</addition>" << std::endl;
}

void add_subtraction(std::ostream& os, int id)
{
	os << "<subtraction id=\"" << id << "\">" << std::endl;

	os << "<minuend>";
	int minuend = rand() % 100 + 1;
	os << minuend;
	os << "</minuend>" << std::endl;

	os << "<subtrahend>";
	int subtrahend = rand() % 100 + 1;
	os << subtrahend;
	os << "</subtrahend>" << std::endl;

	os << "</subtraction>" << std::endl;
}

void add_multiplication(std::ostream& os, int id)
{
	os << "<multiplication  id=\"" << id << "\">" << std::endl;

	for (int i = 0; i < rand() % 5 + 2; ++i)
	{
		os << "<factor>";
		int factor = rand() % 100 + 1;
		os << factor;
		os << "</factor>" << std::endl;
	}

	os << "</multiplication>" << std::endl;
}

void add_division(std::ostream& os, int id)
{
	os << "<division  id=\"" << id << "\">" << std::endl;

	os << "<dividend>";
	int dividend = rand() % 100 + 1;
	os << dividend;
	os << "</dividend>" << std::endl;

	os << "<divisor>";
	int divisor = rand() % 100 + 1;
	os << divisor;
	os << "</divisor>" << std::endl;

	os << "</division>" << std::endl;
}


int main()
{
	//pugi::xml_document doc;
	//if (!doc.load_file("data0002.xml"))
	//	return -1;

	{
		std::ofstream file("file.xml");
		generate_xml(file);
	}

	pugi::xml_document doc;
	if (!doc.load_file("file.xml"))
		return -1;

	std::cout << std::boolalpha;
	for (pugi::xml_node node = doc.child("expressions").first_child(); node; node = node.next_sibling())
	{
		std::cout << node.name() << std::endl;
		std::cout << calculate(node) << std::endl;
	}

	std::cin.get();
	return 0;
}