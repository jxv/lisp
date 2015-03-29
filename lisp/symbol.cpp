#include "symbol.h"
#include "error.h"
#include "environment.h"
#include "nil.h"

using std::shared_ptr;

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

bool Symbol::eq(shared_ptr<Object> obj) const
{
    try
    {
        return m_name == Symbol::name(obj);
    }
    catch(lisp::Error)
    {
    }
    return false;
}

const std::string& Symbol::name() const
{
    return m_name;
}

shared_ptr<Object> Symbol::eval(shared_ptr<Environment> env)
{
    auto obj = env->get(m_name);
    if (obj == Nil::get())
    {
        throw Error::with_object("can't find symbol", *this);
    }
    return obj;
}

shared_ptr<Symbol> Symbol::to(shared_ptr<Object> obj)
{
    if (obj->type() != Type::Symbol)
    {
        throw Error("not symbol");
    }
    return std::dynamic_pointer_cast<Symbol>(obj);
}

const std::string &Symbol::name(shared_ptr<Object> obj)
{
    return Symbol::to(obj)->name();
}

}
