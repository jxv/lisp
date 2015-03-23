#ifndef LISP_ERROR_H
#define LISP_ERROR_H

#include <string>
#include <exception>
#include "object.h"

namespace lisp
{

class Error : public std::exception
{
public:
    Error(const std::string &msg);
    const char * what();
    const std::string &message() const;
    static Error with_object(const std::string &pre_msg, lisp::Object &obj);
private:
    std::string m_message;
};

}

#endif // LISP_ERROR_H
