#include <sstream>
#include "function.h"
#include "environment.h"
#include "list.h"
#include "symbol.h"
#include "error.h"

namespace lisp
{

Type Function::type() const
{
    return Type::Function;
}

CppFunction::CppFunction(std::shared_ptr<Object> (*fn)(std::shared_ptr<Object>))
    : std::function<std::shared_ptr<Object>(std::shared_ptr<Object>)>(fn)
{
}

CppFunction::CppFunction(std::shared_ptr<Object> (*fn)(std::shared_ptr<Object>), const std::string &name)
    : std::function<std::shared_ptr<Object>(std::shared_ptr<Object>)>(fn)
    , m_name(name)
{
}

void CppFunction::write(std::ostream &os) const
{
    if (m_name.length() == 0)
    {
        os << "<fn>";
    }
    else
    {
        os << "<fn: ";
        os << m_name;
        os << ">";
    }
}
    
bool CppFunction::eq(std::shared_ptr<Object> obj) const
{
    return false;
}

std::shared_ptr<Object> CppFunction::apply(std::shared_ptr<Object> obj)
{
    return (*this)(obj);
}

CppFunction &CppFunction::operator =(const CppFunction &fn)
{
    *this = fn;
    return *this;
}

CppFunction &CppFunction::operator =(const std::function<std::shared_ptr<Object> (std::shared_ptr<Object>)> &fn)
{
    *this = fn;
    return *this;
}

Lambda::Lambda(std::shared_ptr<Object> params, std::shared_ptr<Object> body, std::shared_ptr<Environment> env)
    : m_body(body)
    , m_env_parent(env)
{
    auto list = List::to(params);
    for (auto it = list->iterator(); !it->is_done(); it->next())
    {
        m_params.push_back(Symbol::name(it->get()));
    }
}

void Lambda::write(std::ostream &os) const
{
    os << "(lam (";
    for (auto it = m_params.cbegin(); it != m_params.cend(); ++it)
    {
        os << *it;
        if (it != --m_params.cend())
        {
            os << " ";
        }
    }
    os << ") ";
    os << m_body;
    os << ")";
}
    
bool Lambda::eq(std::shared_ptr<Object> obj) const
{
    return false;
}

std::shared_ptr<Object> Lambda::apply(std::shared_ptr<Object> obj)
{
    auto env = std::shared_ptr<Environment>(new Environment(m_env_parent));
    auto args = List::to(obj);
    if (args->size() != m_params.size())
    {
        std::ostringstream oss;
        oss << "lambda: wrong number of arguments:";
        oss << "expected: " << m_params.size() << ", ";
        oss << "given: " << args->size();
        throw Error(oss.str());
    }
    auto param = m_params.begin();
    for (auto arg = args->iterator(); !arg->is_done(); arg->next())
    {
        env->define(*param, arg->get());
        param++;
    }
    return lisp::eval(env, m_body);
}

}
