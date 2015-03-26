#include "empty.h"
#include "error.h"
#include "symbol.h"
#include "environment.h"
#include "boolean.h"
#include "function.h"
#include "nil.h"

namespace lisp
{

Empty::Empty()
{
}

void Empty::write(std::ostream &os) const
{
    os << "()";
}

std::shared_ptr<Object> Empty::get()
{
    static auto singleton = std::shared_ptr<Object>(new Empty);
    return singleton;
}

std::shared_ptr<Object> Empty::car() const
{
    throw Error("can't car empty list");
}

std::shared_ptr<Object> Empty::cdr() const
{
    throw Error("can't cdr empty list");
}

std::shared_ptr<Object> Empty::eval(std::shared_ptr<Environment>)
{
    throw Error("can't eval empty list");
}

unsigned int Empty::size() const
{
    return 0;
}

std::unique_ptr<Iterator> Empty::iterator() const
{
    throw Error("can't iterate empty list");
}

}
