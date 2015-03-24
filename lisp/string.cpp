#include "string.h"
#include "error.h"
#include <cstring>

namespace lisp
{

String::String(const std::string &v)
    : m_value(v)
{
}

std::shared_ptr<String> String::from_escape_chars(const std::string &v)
{
    std::string str;
    // Absolute minimum required size. Will likely still grow.
    str.reserve(v.size() / 2);
    bool escape = false;
    for (auto it = v.cbegin(); it != v.cend(); ++it)
    {
        if (!escape)
        {
            if (*it == '\\')
            {
                escape = true;
            }
            else
            {
                str.push_back(*it);
            }
        }
        else
        {
            switch (*it)
            {
            case 'a':
            {
                str.push_back('\a');
                break;
            }
            case 'b':
            {
                str.push_back('\b');
                break;
            }
            case 'f':
            {
                str.push_back('\f');
                break;
            }
            case 'n':
            {
                str.push_back('\n');
                break;
            }
            case 'r':
            {
                str.push_back('\r');
                break;
            }
            case 't':
            {
                str.push_back('\t');
                break;
            }
            case 'v':
            {
                str.push_back('\v');
                break;
            }
            case '\\':
            {
                str.push_back('\\');
                break;
            }
            case '\'':
            {
                str.push_back('\'');
                break;
            }
            case '\"':
            {
                str.push_back('\"');
                break;
            }
            case '\?':
            {
                str.push_back('\?');
                break;
            }
            default:
            {
                throw Error("not escape char");
            }
            }
            escape = false;
        }
    }
    if (escape)
    {
        throw Error("expected escape char");
    }
    return std::shared_ptr<String>(new String(str));
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

void String::show(std::ostream &os) const
{
    os << m_value;
}


Type String::type() const
{
    return Type::String;
}

const std::string &String::value() const
{
    return m_value;
}

std::shared_ptr<String> String::to(std::shared_ptr<Object> obj)
{
    if (obj->type() != Type::String)
    {
        throw Error("not string");
    }
    return std::dynamic_pointer_cast<String>(obj);
}

const std::string &String::value(std::shared_ptr<Object> obj)
{
    return String::to(obj)->value();
}

}
