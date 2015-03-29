#include "quote.h"
#include "list.h"
#include "environment.h"
#include "error.h"

using std::shared_ptr;

namespace lisp
{

Quote::Quote(shared_ptr<Object> obj)
    : m_object(obj)
{
}

void Quote::write(std::ostream &os) const
{
    os << "(quote " << m_object << ")";
}
    
bool Quote::eq(shared_ptr<Object> obj) const
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

shared_ptr<Object> Quote::eval(shared_ptr<Environment>)
{
    return m_object;
}

shared_ptr<Quote> Quote::to(shared_ptr<Object> obj)
{
    if (obj->type() != Type::Quote)
    {
        throw Error::with_object("not quote", *obj);
    }
    return std::dynamic_pointer_cast<Quote>(obj);
}

}
