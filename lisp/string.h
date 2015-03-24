#ifndef LISP_STRING_H
#define LISP_STRING_H

#include "object.h"

namespace lisp
{

class String : public Object
{
public:
    String(const std::string &);
    static std::shared_ptr<String> from_escape_chars(const std::string &);
    void write(std::ostream &) const;
    void show(std::ostream &) const;
    Type type() const;
    const std::string &value() const;
    static std::shared_ptr<String> to(std::shared_ptr<Object> obj);
    static const std::string &value(std::shared_ptr<Object> obj);
private:
    const std::string m_value;
};

}

#endif // LISP_STRING_H
