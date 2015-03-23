#include "string.h"
#include "error.h"
#include <cstring>

namespace lisp
{

String::String(const std::string &v)
    : m_value(v)
{
}

void String::display(std::ostream &os) const
{
    os << m_value;
}

void String::write(std::ostream &os) const
{
    os << '"';
    for(const char c : m_value) {
        switch (c)
        {
        case '\a':
        {
            os << "\\a";
            break;
        }
        case '\b':
        {
            os << "\\b";
            break;
        }
        case '\f':
        {
            os << "\\f";
            break;
        }
        case '\n':
        {
            os << "\\n";
            break;
        }
        case '\r':
        {
            os << "\\r";
            break;
        }
        case '\t':
        {
            os << "\\t";
            break;
        }
        case '\v':
        {
            os << "\\v";
            break;
        }
        case '\\':
        {
            os << "\\\\";
            break;
        }
        case '\'':
        {
            os << "\\'";
            break;
        }
        case '\"':
        {
            os << "\\\"";
            break;
        }
        case '\?':
        {
            os << "\\\?";
            break;
        }
        default:
        {
            if (std::isprint(c))
            {
                os << c;
            }
            else
            {
                os << '\\' << static_cast<int>(c);
            }
            break;
        }
        }
    }
    os << '"';
}

Type String::type() const
{
    return Type::String;
}

const std::string &String::value() const
{
    return m_value;
}

std::shared_ptr<String> to_string(std::shared_ptr<Object> obj)
{
    if (obj->type() != Type::String)
    {
        throw Error("not string");
    }
    return std::dynamic_pointer_cast<String>(obj);
}

const std::string &get_string(std::shared_ptr<Object> obj)
{
    return to_string(obj)->value();
}

}
