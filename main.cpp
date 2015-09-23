#include <iostream>

#include "thirdparty\pugixml\src\pugixml.hpp"

double calculate(pugi::xml_node node, const int id)
{
	double result = node.first_child().text().as_double();
	for (node = node.first_child().next_sibling(); node; node = node.next_sibling())
	{
		switch (id)
		{
		case 1:
			result += node.text().as_double();
			break;
		case 2:
			result -= node.text().as_double();
			break;
		case 3:
			result *= node.text().as_double();
			break;
		case 4:
			result /= node.text().as_double();
			break;
		}
	}
	return result;
}

int main()
{
	pugi::xml_document doc;
	if (!doc.load_file("data0001.xml"))
		return -1;

	for (pugi::xml_node node = doc.child("expressions").first_child(); node; node = node.next_sibling())
	{
		switch (node.attribute("id").as_int())
		{
		case '1':
			std::cout << calculate(node, 1) << "\n";
			break;
		case '2':
			std::cout << calculate(node, 2) << "\n";
			break;
		case '3':
			std::cout << calculate(node, 3) << "\n";
			break;
		case '4':
			std::cout << calculate(node, 4) << "\n";
			break;
		}
	}

std::cin.get();
return 0;
}