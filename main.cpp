#include <iostream>
#include <fstream>
#include "xml_valuator.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "usage: " << std::string(argv[0]) << " data0001.xml" << std::endl;
		return 0;
	}
	const std::string fname = argv[1];
	xml_valuator_t valuator(std::cout);
	valuator.parse(std::ifstream(fname));

	return 0;
}