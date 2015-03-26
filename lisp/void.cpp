#include "void.h"

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
    
bool Void::eq(std::shared_ptr<Object> obj) const
{
    return Void::get() == obj;
}

std::shared_ptr<Void> Void::get()
{
    static auto singleton = std::shared_ptr<Void>(new Void);
    return singleton;
}

}
