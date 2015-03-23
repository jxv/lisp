#include "boolean.h"
#include "error.h"

namespace lisp
{

Boolean::Boolean(bool v)
    : m_value(v)
{
}

Boolean::Boolean()
    : Boolean(false)
{
}

void Boolean::write(std::ostream &os) const
{
    os << (m_value ? "#t" : "#f");
}

Type Boolean::type() const
{
    return Type::Boolean;
}

bool Boolean::value() const
{
    return m_value;
}

bool Boolean::value(std::shared_ptr<Object> obj)
{
    return Boolean::to(obj)->value();
}

std::shared_ptr<Boolean> Boolean::to(std::shared_ptr<Object> obj)
{
    if (obj->type() != Type::Boolean)
    {
        throw Error::with_object("not boolean", *obj);
    }
    return std::dynamic_pointer_cast<Boolean>(obj);
}

}