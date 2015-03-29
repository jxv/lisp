#include "boolean.h"
#include "error.h"

using std::shared_ptr;

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

bool Boolean::eq(shared_ptr<Object> obj) const
{
    try
    {
        return m_value == Boolean::value(obj);
    }
    catch(lisp::Error)
    {
    }
    return false;
}

bool Boolean::value() const
{
    return m_value;
}

bool Boolean::value(shared_ptr<Object> obj)
{
    return Boolean::to(obj)->value();
}

shared_ptr<Boolean> Boolean::to(shared_ptr<Object> obj)
{
    if (obj->type() != Type::Boolean)
    {
        throw Error::with_object("not boolean", *obj);
    }
    return std::dynamic_pointer_cast<Boolean>(obj);
}

}
