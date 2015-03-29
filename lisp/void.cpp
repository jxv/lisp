#include "void.h"

using std::shared_ptr;

namespace lisp
{

void Void::write(std::ostream &os) const
{
    os << "<void>";
}

Type Void::type() const
{
    return Type::Void;
}
    
bool Void::eq(shared_ptr<Object> obj) const
{
    return Void::get() == obj;
}

shared_ptr<Void> Void::get()
{
    static auto singleton = shared_ptr<Void>(new Void);
    return singleton;
}

}
