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

Empty::Empty()
{
}

void Empty::write(std::ostream &os) const
{
    os << "()";
}

shared_ptr<Object> Empty::get()
{
    static auto singleton = shared_ptr<Object>(new Empty);
    return singleton;
}

shared_ptr<Object> Empty::car() const
{
    throw Error("can't car empty list");
}

shared_ptr<Object> Empty::cdr() const
{
    throw Error("can't cdr empty list");
}

shared_ptr<Object> Empty::eval(shared_ptr<Environment>)
{
    throw Error("can't eval empty list");
}

unsigned int Empty::size() const
{
    return 0;
}

unique_ptr<Iterator> Empty::iterator() const
{
    return unique_ptr<Iterator>(new EmptyIterator());
}

EmptyIterator::EmptyIterator()
{
}

void EmptyIterator::next()
{
    throw Error("empty list cannot iterate to next item");
}

bool EmptyIterator::is_done() const
{
    return true;
}

bool EmptyIterator::is_last() const
{
    return false;
}

shared_ptr<Object> EmptyIterator::get() const
{
    throw Error("can't get element from empty list");
}

}
