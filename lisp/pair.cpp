#include "pair.h"
#include "empty.h"
#include "error.h"
#include "symbol.h"
#include "environment.h"
#include "boolean.h"
#include "function.h"
#include "nil.h"

using std::shared_ptr;

namespace lisp
{

Pair::Pair(shared_ptr<Object> car, shared_ptr<Object> cdr)
    : m_car(car)
    , m_cdr(cdr)
{
}

shared_ptr<Object> Pair::car() const
{
    return m_car;
}

shared_ptr<Object> Pair::cdr() const
{
    return m_cdr;
}

shared_ptr<Object> Pair::eval(shared_ptr<Environment> env)
{
    try
    {
        auto fn = lisp::eval(env, Symbol::to(m_car));
        return fn->apply(m_cdr);
    }
    catch(lisp::Error)
    {
        throw Error("can't eval list");
    }
}

unsigned int Pair::size() const
{
    try
    {
       return List::to(m_cdr)->size() + 1;
    }
    catch(lisp::Error)
    {
    }
    return 0; 
}

std::unique_ptr<lisp::Iterator> Pair::iterator() const
{
    new shared_ptr<lisp::Iterator>(new PairIterator(m_car, m_cdr));
}

PairIterator::PairIterator(shared_ptr<Object> car, shared_ptr<Object> cdr)
    : m_car(car)
    , m_cdr(cdr)
{
}

void PairIterator::next()
{

    if (is_done())
    {
        throw Error("can't next, pair iterator is done");
    }
    else if (is_last())
    {
        m_car = m_cdr;
        m_cdr = Empty::get();
    }
    else
    {
        try
        {
            auto cdr = List::to(m_cdr)->cdr();
            m_car = m_cdr;
            m_cdr = cdr;
        }
        catch(lisp::Error)
        {
            throw Error("can't next, cdr is not a list");
        }
    }
}

bool PairIterator::is_done() const
{
    return m_car == Empty::get() && m_cdr == Empty::get();
}

bool PairIterator::is_last() const
{
    return m_car != Empty::get() && m_cdr == Empty::get();
}

shared_ptr<Object> PairIterator::get() const
{
    return m_car;
}

}
