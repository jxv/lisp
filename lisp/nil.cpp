#include "nil.h"

using std::shared_ptr;

namespace lisp
{

void Nil::write(std::ostream &os) const
{
    os << "<nil>";
}
    
bool Nil::eq(shared_ptr<Object> obj) const
{
    return Nil::get() == obj;
}

Type Nil::type() const
{
    return Type::Nil;
}

shared_ptr<Nil> Nil::get()
{
    static auto nil = shared_ptr<Nil>(new Nil());
    return nil;
}

Nil::Nil()
{
}

}
