#include <iostream>
#include "xml_valuator.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "usage: " << std::string(argv[0]) << " fname.xml" << std::endl;
		return 0;
	}
	const std::string fname = argv[1];
	xml_valuator valuator(std::cout);
	valuator.calculate(fname);

	return 0;
}