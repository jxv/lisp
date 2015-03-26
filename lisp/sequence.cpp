#include "sequence.h"
#include "error.h"
#include "void.h"

namespace lisp
{

Sequence::Sequence(std::vector<std::shared_ptr<Object> > &statements)
    : m_statements(statements)
{
}

void Sequence::write(std::ostream &os) const
{
    os << "( begin ";
    for (auto it = m_statements.begin(); it != m_statements.end(); ++it)
    {
        os << *it;
        os << " ";
    }
    os << ")";
}

bool Sequence::eq(std::shared_ptr<Object> obj) const
{
    return false;
}

Type Sequence::type() const
{
    return Type::Sequence;
}

std::shared_ptr<Object> Sequence::eval(std::shared_ptr<Environment> env)
{
    std::shared_ptr<Object> last = Void::get();
    for (auto it = m_statements.begin(); it != m_statements.end(); ++it)
    {
        last = lisp::eval(env, *it);
    }
    return last;
}
    
std::shared_ptr<Sequence> Sequence::to(std::shared_ptr<Object> obj)
{
    if (obj->type() != Type::Sequence)
    {
        throw Error::with_object("not sequence", *obj);
    }
    return std::dynamic_pointer_cast<Sequence>(obj);
}

}
