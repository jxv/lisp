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
private:
    const std::string m_name;
};

std::shared_ptr<Symbol> to_symbol(std::shared_ptr<Object> obj);
const std::string &get_symbol(std::shared_ptr<Object> obj);

}

#endif // LISP_SYMBOL_H
