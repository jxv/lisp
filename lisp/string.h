#ifndef LISP_STRING_H
#define LISP_STRING_H

#include "object.h"

namespace lisp
{

class String : public Object
{
public:
    String(const std::string &);
    void display(std::ostream &) const;
    void write(std::ostream &) const;
    Type type() const;
    const std::string &value() const;
private:
    const std::string m_value;
};

std::shared_ptr<String> to_string(std::shared_ptr<Object> obj);
const std::string &get_string(std::shared_ptr<Object> obj);

}

#endif // LISP_STRING_H
