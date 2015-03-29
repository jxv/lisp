#include "sequence.h"
#include "error.h"
#include "void.h"

using std::shared_ptr;

namespace lisp
{

Sequence::Sequence(std::vector<shared_ptr<Object> > &statements)
    : m_statements(statements)
{
}

void Sequence::write(std::ostream &os) const
{
    os << "(do ";
    for (auto it = m_statements.begin(); it != m_statements.end(); ++it)
    {
        os << *it;
        if (*it != m_statements.back())
        {
            os << " ";
        }
    }
    os << ")";
}

bool Sequence::eq(shared_ptr<Object> obj) const
{
    return false;
}

Type Sequence::type() const
{
    return Type::Sequence;
}

shared_ptr<Object> Sequence::eval(shared_ptr<Environment> env)
{
    shared_ptr<Object> last = Void::get();
    for (auto it = m_statements.begin(); it != m_statements.end(); ++it)
    {
        last = lisp::eval(env, *it);
    }
    return last;
}
    
shared_ptr<Sequence> Sequence::to(shared_ptr<Object> obj)
{
    if (obj->type() != Type::Sequence)
    {
        throw Error::with_object("not sequence", *obj);
    }
    return std::dynamic_pointer_cast<Sequence>(obj);
}

}
