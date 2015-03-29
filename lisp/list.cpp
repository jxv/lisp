#include "list.h"
#include "empty.h"
#include "linkedlist.h"
#include "error.h"
#include "symbol.h"
#include "environment.h"
#include "boolean.h"
#include "function.h"
#include "nil.h"

using std::shared_ptr;

namespace lisp
{

void List::write(std::ostream &os) const
{
    os << "(";
    for (auto it = iterator(); !it->is_done(); it->next())
    {
        os << it->get();
        if (!it->is_last())
        {
            os << " ";
        }
    }
    os << ")";

}

Type List::type() const
{
    return Type::List;
}

bool List::eq(shared_ptr<Object> obj) const
{
    try
    {
        auto list = List::to(obj);
        if (size() == 0 && list->size() == 0)
        {
            return true;
        }
    }
    catch(lisp::Error)
    {
    }
    return false;
}

shared_ptr<List> List::to(shared_ptr<Object> obj)
{
    if (obj->type() != Type::List)
    {
        throw Error::with_object("not list", *obj);
    }
    return std::dynamic_pointer_cast<List>(obj);
}

shared_ptr<Object> cons(shared_ptr<Object> car, shared_ptr<Object> cdr)
{
    if (cdr->type() != Type::List)
    {
        throw Error("cons's cdr must be a of type list");
    }
    if (cdr == Empty::get())
    {
        std::list<shared_ptr<Object>> items = { car };
        return shared_ptr<LinkedList>(new LinkedList(items));
    }
    std::list<shared_ptr<Object>> items;
    auto list = std::dynamic_pointer_cast<lisp::List>(cdr);
    items.push_front(car);
    for (auto it = list->iterator(); !it->is_done(); it->next())
    {
        items.push_back(it->get());
    }
    return shared_ptr<LinkedList>(new LinkedList(items));
}

}
