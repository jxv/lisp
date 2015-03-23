#include "list.h"
#include "error.h"
#include "symbol.h"
#include "environment.h"
#include "boolean.h"
#include "function.h"

namespace lisp
{

void List::write(std::ostream &os) const
{
    os << "(";
    for (auto it = iterator(); !it->is_done(); it->next())
    {
        if (it->is_last())
        {
            if (!is_last_empty())
            {
                os << ". ";
            }
            os << it->get();
        }
        else
        {
            os << it->get();
            os << " ";
        }
    }
    os << ")";

}

Type List::type() const
{
    return Type::List;
}

std::shared_ptr<List> List::to(std::shared_ptr<Object> obj)
{
    if (obj->type() != Type::List)
    {
        throw Error::with_object("not list", *obj);
    }
    return std::dynamic_pointer_cast<List>(obj);
}

LinkedList::LinkedList(std::list<std::shared_ptr<Object>> &items, bool is_last_empty)
    : m_is_last_empty(is_last_empty)
    , m_items(items)
{
}

std::shared_ptr<Object> LinkedList::car() const
{
    return m_items.front();
}

std::shared_ptr<Object> LinkedList::cdr() const
{
    if (m_items.size() == 0)
    {
        if (m_is_last_empty)
        {
            return Empty::get();
        }
        else
        {
            throw Error("pair expected");
        }
    }
    auto items = m_items;
    items.pop_front();
    return std::shared_ptr<LinkedList>(new LinkedList(items, m_is_last_empty));
}

std::shared_ptr<Object> LinkedList::eval(std::shared_ptr<Environment> env)
{
    std::shared_ptr<Object> obj = nullptr;
    if ((obj = eval_quote()) != nullptr) return obj;
    if ((obj = eval_set(env)) != nullptr) return obj;
    if ((obj = eval_if(env)) != nullptr) return obj;
    if ((obj = eval_cond(env)) != nullptr) return obj;
    if ((obj = eval_define(env)) != nullptr) return obj;
    if ((obj = eval_lambda(env)) != nullptr) return obj;
    if ((obj = eval_cons(env)) != nullptr) return obj;
    if ((obj = eval_car(env)) != nullptr) return obj;
    if ((obj = eval_cdr(env)) != nullptr) return obj;
    if ((obj = eval_function(env)) != nullptr) return obj;
    throw Error("can't eval list");
}

unsigned int LinkedList::size() const
{
    return m_items.size();
}

std::unique_ptr<lisp::Iterator> LinkedList::iterator() const
{
    auto it = new LinkedList_Iterator(m_items.begin(), m_items.end());
    return std::unique_ptr<lisp::Iterator>(it);
}

bool LinkedList::is_last_empty() const
{
    return m_is_last_empty;
}

void LinkedList::set_last_empty()
{
    m_is_last_empty = true;
}

std::shared_ptr<Object> LinkedList::eval_quote() const
{
    if (!(m_items.size() == 2)) return nullptr;
    auto it = m_items.begin();
    auto op = *it;
    it++;
    auto obj = *it;
    if (!(op->type() == Type::Symbol)) return nullptr;
    if (!(Symbol::name(op) == "quote")) return nullptr;
    return obj;
}

std::shared_ptr<Object> LinkedList::eval_set(std::shared_ptr<Environment> env) const
{
    if (!(m_items.size() == 3)) return nullptr;
    auto it = m_items.begin();
    auto op = *it;
    it++;
    auto key = *it;
    it++;
    auto body = *it;
    if (!(op->type() == Type::Symbol)) return nullptr;
    if (!(Symbol::name(op) == "set!")) return nullptr;
    return env->set(Symbol::name(key), lisp::eval(env, body));
}

std::shared_ptr<Object> LinkedList::eval_if(std::shared_ptr<Environment> env) const
{
    if (!(m_items.size() == 4)) return nullptr;
    auto it = m_items.begin();
    auto op = *it;
    it++;
    auto pred = *it;
    it++;
    auto on_true = *it;
    it++;
    auto on_false = *it;
    if (!(op->type() == Type::Symbol)) return nullptr;
    if (!(Symbol::name(op) == "if")) return nullptr;
    if (Boolean::value(lisp::eval(env, pred)))
    {
        return lisp::eval(env, lisp::eval(env, on_true));
    }
    else
    {
        return lisp::eval(env, lisp::eval(env, on_false));
    }
}

std::shared_ptr<Object> LinkedList::eval_cond(std::shared_ptr<Environment> env) const
{
    if (!(m_items.size() >= 2)) return nullptr;
    auto it = m_items.begin();
    auto op = *it;
    if (!(op->type() == Type::Symbol)) return nullptr;
    if (!(Symbol::name(op) == "cond")) return nullptr;
    it++;
    for (; it != m_items.end(); ++it)
    {
        auto list = List::to(*it);
        if (list->size() != 2)
        {
            throw Error("condition has wrong list size");
        }
        auto list_it = list->iterator();
        if (Boolean::value(lisp::eval(env, list_it->get())))
        {
            list_it->next();
            return lisp::eval(env, list_it->get());
        }
    }
    throw Error("all conditions failed");
}

std::shared_ptr<Object> LinkedList::eval_define(std::shared_ptr<Environment> env) const
{
    if (!(m_items.size() == 3)) return nullptr;
    auto it = m_items.begin();
    auto op = *it;
    it++;
    auto key = *it;
    it++;
    auto body = *it;
    if (!(op->type() == Type::Symbol)) return nullptr;
    if (!(Symbol::name(op) == "def")) return nullptr;
    if (key->type() == Type::Symbol) 
    {
        return env->define(Symbol::name(key), lisp::eval(env, body));
    }
    else if (key->type() == Type::List)
    {
        auto fn_params = List::to(key);
        if (fn_params->size() >= 1)
        {
            auto it = fn_params->iterator();
            auto fn = it->get();
            if (fn->type() != Type::Symbol) return nullptr;

            std::list<std::shared_ptr<Object>> params;
            for (it->next(); !it->is_done(); it->next())
            {
                params.push_back(it->get());
            }
            auto lambda = std::shared_ptr<Object>(new Lambda(std::shared_ptr<Object>(new LinkedList(params)), body, env));
            return env->define(Symbol::name(fn), lambda);
        }
        return nullptr;
    }
    return nullptr;
}

std::shared_ptr<Object> LinkedList::eval_lambda(std::shared_ptr<Environment> env) const
{
    if (!(m_items.size() == 3)) return nullptr;
    auto it = m_items.begin();
    auto op = *it;
    it++;
    auto params = *it;
    it++;
    auto body = *it;
    if (!(op->type() == Type::Symbol)) return nullptr;
    if (!(Symbol::name(op) == "lam")) return nullptr;
    if (!(params->type() == Type::List)) return nullptr;
    return std::shared_ptr<Object>(new Lambda(params, body, env));
}

std::shared_ptr<Object> LinkedList::eval_function(std::shared_ptr<Environment> env) const
{
    if (!(m_items.size() >= 1)) return nullptr;
    auto it = m_items.begin();
    auto op = *it;
    it++;
    std::list<std::shared_ptr<Object>> items;
    for (; it != m_items.end(); ++it)
    {
        items.push_back(lisp::eval(env, *it));
    }
    auto args = std::shared_ptr<LinkedList>(new LinkedList(items));
    return lisp::eval(env, op)->apply(args);
}

std::shared_ptr<Object> LinkedList::eval_cons(std::shared_ptr<Environment> env) const
{
    if (!(m_items.size() == 3)) return nullptr;
    auto it = m_items.begin();
    auto op = *it;
    it++;
    auto fst = *it;
    it++;
    auto snd = *it;
    if (!(op->type() == Type::Symbol)) return nullptr;
    if (!(Symbol::name(op) == "cons")) return nullptr;
    return lisp::cons(lisp::eval(env, fst), lisp::eval(env, snd));
}

std::shared_ptr<Object> LinkedList::eval_car(std::shared_ptr<Environment> env) const
{
    if (!(m_items.size() == 2)) return nullptr;
    auto it = m_items.begin();
    auto op = *it;
    it++;
    auto list = *it;
    if (!(op->type() == Type::Symbol)) return nullptr;
    if (!(Symbol::name(op) == "car")) return nullptr;
    return List::to(lisp::eval(env, list))->car();
}

std::shared_ptr<Object> LinkedList::eval_cdr(std::shared_ptr<Environment> env) const
{
    if (!(m_items.size() == 2)) return nullptr;
    auto it = m_items.begin();
    auto op = *it;
    it++;
    auto list = *it;
    if (!(op->type() == Type::Symbol)) return nullptr;
    if (!(Symbol::name(op) == "cdr")) return nullptr;
    return List::to(lisp::eval(env, list))->cdr();
}

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
    return nullptr;
}

bool Empty::is_last_empty() const
{
    return false;
}

void Empty::set_last_empty()
{
}

LinkedList_Iterator::LinkedList_Iterator(std::list<std::shared_ptr<Object>>::const_iterator start, std::list<std::shared_ptr<Object>>::const_iterator last)
    : m_current(start)
    , m_last(last)
    , m_end(last)
{
    m_last--;
}

void LinkedList_Iterator::next()
{
    m_current++;
}

bool LinkedList_Iterator::is_done() const
{
    return m_current == m_end;
}

bool LinkedList_Iterator::is_last() const
{
    return m_current == m_last;
}

std::shared_ptr<Object> LinkedList_Iterator::get() const
{
    return *m_current;
}

std::shared_ptr<Object> cons(std::shared_ptr<Object> car, std::shared_ptr<Object> cdr)
{
    if (cdr == Empty::get())
    {
        std::list<std::shared_ptr<Object>> items = { car };
        return std::shared_ptr<LinkedList>(new LinkedList(items, true));
    }
    if (cdr->type() == Type::List)
    {
        std::list<std::shared_ptr<Object>> items;
        auto list = std::dynamic_pointer_cast<lisp::List>(cdr);
        for (auto it = list->iterator(); !it->is_done(); it->next())
        {
            items.push_back(it->get());
        }
        items.push_front(car);
        return std::shared_ptr<LinkedList>(new LinkedList(items, list->is_last_empty()));
    }
    std::list<std::shared_ptr<Object>> items = { car, cdr };
    return std::shared_ptr<LinkedList>(new LinkedList(items, false));
}

}
