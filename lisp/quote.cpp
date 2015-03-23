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

void Quote::display(std::ostream &os) const
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

}
