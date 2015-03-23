#include "symbol.h"
#include "error.h"
#include "environment.h"

namespace lisp
{

Symbol::Symbol(const std::string &name)
    : m_name(name)
{
}

void Symbol::write(std::ostream &os) const
{
    os << m_name;
}

Type Symbol::type() const
{
    return Type::Symbol;
}

std::shared_ptr<Symbol> to_symbol(std::shared_ptr<Object> obj)
{
    if (obj->type() != Type::Symbol)
    {
        throw Error("not symbol");
    }
    return std::dynamic_pointer_cast<Symbol>(obj);
}

const std::string& Symbol::name() const
{
    return m_name;
}

std::shared_ptr<Object> Symbol::eval(std::shared_ptr<Environment> env)
{
    auto obj = env->get(m_name);
    if (obj == nullptr)
    {
        throw Error::with_object("can't find symbol", *this);
    }
    return obj;
}

const std::string &get_symbol(std::shared_ptr<Object> obj)
{
    return to_symbol(obj)->name();
}

}
