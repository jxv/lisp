#include "nil.h"

namespace lisp
{

void Nil::write(std::ostream &os) const
{
    os << "<nil>";
}
    
bool Nil::eq(std::shared_ptr<Object> obj) const
{
    return Nil::get() == obj;
}

Type Nil::type() const
{
    return Type::Nil;
}

std::shared_ptr<Nil> Nil::get()
{
    static auto nil = std::shared_ptr<Nil>(new Nil());
    return nil;
}

Nil::Nil()
{
}

}
