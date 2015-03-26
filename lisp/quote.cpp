#include "quote.h"
#include "list.h"
#include "environment.h"
#include "error.h"

namespace lisp
{

Quote::Quote(std::shared_ptr<Object> obj)
    : m_object(obj)
{
    try
    {
        List::to(m_object)->set_last_empty();
    }
    catch(lisp::Error)
    {
    }
}

void Quote::write(std::ostream &os) const
{
    os << "(quote " << m_object << ")";
}
    
bool Quote::eq(std::shared_ptr<Object> obj) const
{
    try
    {
        return m_object->eq(Quote::to(obj)->m_object);
    }
    catch(lisp::Error)
    {
    }
    return false;
}

void Quote::show(std::ostream &os) const
{
    os << m_object;
}

Type Quote::type() const
{
    return Type::Quote;
}

std::shared_ptr<Object> Quote::eval(std::shared_ptr<Environment>)
{
    return m_object;
}

std::shared_ptr<Quote> Quote::to(std::shared_ptr<Object> obj)
{
    if (obj->type() != Type::Quote)
    {
        throw Error::with_object("not quote", *obj);
    }
    return std::dynamic_pointer_cast<Quote>(obj);
}

}
