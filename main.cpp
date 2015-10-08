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

    std::ofstream ofs("result.xml");

    xml_valuator_t valuator(ofs);
    valuator.parse(std::ifstream(fname));

    return 0;
}