#include "sax_parser.h"
#include <vector>
#include <istream>

#include <string>
#include <iostream>

sax_parser_t::sax_parser_t()
{
    parser = XML_ParserCreate(nullptr);

    XML_SetElementHandler(parser, start, end);
    XML_SetCharacterDataHandler(parser, value);
    XML_SetUserData(parser, this);

}

sax_parser_t::~sax_parser_t()
{
    XML_ParserFree(parser);
}

void sax_parser_t::parse(std::istream& is)
{
    std::vector<char> part(100ull * 1024ull * 1024ull);
    while (is)
    {
        is.read(&part[0], part.size());
        if (XML_Parse(parser, &part[0], is.gcount(), is.gcount() < part.size()) == 0)
        {
            const int code = XML_GetErrorCode(parser);
            const std::string msg(XML_ErrorString((XML_Error)code));
            throw std::logic_error(msg);
            break;
        }
    }
}

void sax_parser_t::start(void* userData, const char* name, const char* args[])
{
    sax_parser_t* sax_parser = reinterpret_cast<sax_parser_t*>(userData);
    sax_parser->open_tag(std::string(name));
    sax_parser->nodes.push(name);
}
void sax_parser_t::value(void* userData, const char* val, int len)
{
    sax_parser_t* sax_parser = reinterpret_cast<sax_parser_t*>(userData);
    sax_parser->value(std::string(val, len));
}
void sax_parser_t::end(void *userData, const char *name)
{
    sax_parser_t* sax_parser = reinterpret_cast<sax_parser_t*>(userData);
    sax_parser->close_tag(std::string(name));
    sax_parser->nodes.pop();
}