#include "sequence.h"

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

Type Sequence::type() const
{
    return Type::Sequence;
}

std::shared_ptr<Object> Sequence::eval(std::shared_ptr<Environment> env)
{
    std::shared_ptr<Object> last = nullptr;
    for (auto it = m_statements.begin(); it != m_statements.end(); ++it)
    {
        last = lisp::eval(env, *it);
    }
    return last;
}

}
