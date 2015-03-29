#include "vectorlist.h"
#include "empty.h"
#include "error.h"
#include "symbol.h"
#include "environment.h"
#include "boolean.h"
#include "function.h"
#include "nil.h"

using std::shared_ptr;
using std::unique_ptr;

namespace lisp
{

VectorList::VectorList(std::vector<shared_ptr<Object>> &items)
    : m_items(items)
{
    if (items.size() < 1)
    {
        throw Error("vector list must contain items");
    }
}

shared_ptr<Object> VectorList::car() const
{
    return m_items.back();
}

shared_ptr<Object> VectorList::cdr() const
{
    if (m_items.size() == 1)
    {
        return Empty::get();
    }
    auto items = m_items;
    items.pop_back();
    return shared_ptr<VectorList>(new VectorList(items));
}

shared_ptr<Object> VectorList::eval(shared_ptr<Environment> env)
{
    throw Error("can't eval vector list");
}
    
unsigned int VectorList::size() const
{
    return m_items.size();
}

unique_ptr<Iterator> VectorList::iterator() const
{
    return unique_ptr<Iterator>(new VectorListIterator(m_items.rbegin(), m_items.rend()));
}

VectorListIterator::VectorListIterator(std::vector<shared_ptr<Object>>::const_reverse_iterator start, std::vector<shared_ptr<Object>>::const_reverse_iterator last)
    : m_current(start)
    , m_last(last)
    , m_end(last)
{
    m_last--;
}

void VectorListIterator::next()
{
    m_current++;
}

bool VectorListIterator::is_done() const
{
    return m_current == m_end;
}

bool VectorListIterator::is_last() const
{
    return m_current == m_last;
}

shared_ptr<Object> VectorListIterator::get() const
{
    return *m_current;
}

}
