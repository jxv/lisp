#include "error.h"
#include <sstream>
#include <string>

namespace lisp
{

Error::Error(const std::string &msg)
    : m_message(msg)
{
}

const char * Error::what()
{
    return m_message.c_str();
}

const std::string &Error::message() const
{
    return m_message;
}

Error Error::with_object(const std::string &pre_msg, Object &obj)
{
    std::string msg = pre_msg + " \'";
    std::ostringstream oss;
    oss << obj;
    msg += oss.str();
    msg += "\'";
    return Error(msg);
}

}
