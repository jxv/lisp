#include "list.h"
#include "empty.h"
#include "linkedlist.h"
#include "error.h"
#include "symbol.h"
#include "environment.h"
#include "boolean.h"
#include "function.h"
#include "nil.h"

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

bool List::eq(std::shared_ptr<Object> obj) const
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

std::shared_ptr<List> List::to(std::shared_ptr<Object> obj)
{
    if (obj->type() != Type::List)
    {
        throw Error::with_object("not list", *obj);
    }
    return std::dynamic_pointer_cast<List>(obj);
}

std::shared_ptr<Object> cons(std::shared_ptr<Object> car, std::shared_ptr<Object> cdr)
{
    if (cdr->type() != Type::List)
    {
        throw Error("cons's cdr must be a of type list");
    }
    if (cdr == Empty::get())
    {
        std::list<std::shared_ptr<Object>> items = { car };
        return std::shared_ptr<LinkedList>(new LinkedList(items));
    }
    std::list<std::shared_ptr<Object>> items;
    auto list = std::dynamic_pointer_cast<lisp::List>(cdr);
    items.push_front(car);
    for (auto it = list->iterator(); !it->is_done(); it->next())
    {
        items.push_back(it->get());
    }
    return std::shared_ptr<LinkedList>(new LinkedList(items));
}

}
