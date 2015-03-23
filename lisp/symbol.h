#ifndef LISP_SYMBOL_H
#define LISP_SYMBOL_H

#include "object.h"

namespace lisp
{

class Symbol : public Object
{
public:
    Symbol(const std::string &name);
    void write(std::ostream &) const;
    Type type() const;
    const std::string &name() const;
    std::shared_ptr<Object> eval(std::shared_ptr<Environment>);
    static std::shared_ptr<Symbol> to(std::shared_ptr<Object> obj);
    static const std::string &name(std::shared_ptr<Object> obj);
private:
    const std::string m_name;
};

}

#endif // LISP_SYMBOL_H
