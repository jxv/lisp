#include "linkedlist.h"
#include "empty.h"
#include "error.h"
#include "symbol.h"
#include "environment.h"
#include "boolean.h"
#include "function.h"
#include "nil.h"

namespace lisp
{

LinkedList::LinkedList(std::list<std::shared_ptr<Object>> &items)
    : m_items(items)
{
    if (items.size() < 1)
    {
        throw Error("linked list must contain items");
    }
}

std::shared_ptr<Object> LinkedList::car() const
{
    return m_items.front();
}

std::shared_ptr<Object> LinkedList::cdr() const
{
    if (m_items.size() == 1)
    {
        return Empty::get();
    }
    auto items = m_items;
    items.pop_front();
    return std::shared_ptr<LinkedList>(new LinkedList(items));
}

std::shared_ptr<Object> LinkedList::eval(std::shared_ptr<Environment> env)
{
    std::shared_ptr<Object> obj = Nil::get();
    if ((obj = eval_quote()) != Nil::get()) return obj;
    if ((obj = eval_set(env)) != Nil::get()) return obj;
    if ((obj = eval_if(env)) != Nil::get()) return obj;
    if ((obj = eval_cond(env)) != Nil::get()) return obj;
    if ((obj = eval_define(env)) != Nil::get()) return obj;
    if ((obj = eval_lambda(env)) != Nil::get()) return obj;
    if ((obj = eval_cons(env)) != Nil::get()) return obj;
    if ((obj = eval_car(env)) != Nil::get()) return obj;
    if ((obj = eval_cdr(env)) != Nil::get()) return obj;
    if ((obj = eval_eq(env)) != Nil::get()) return obj;
    if ((obj = eval_function(env)) != Nil::get()) return obj;
    throw Error("can't eval list");
}

unsigned int LinkedList::size() const
{
    return m_items.size();
}

std::unique_ptr<lisp::Iterator> LinkedList::iterator() const
{
    auto it = new LinkedListIterator(m_items.begin(), m_items.end());
    return std::unique_ptr<lisp::Iterator>(it);
}

std::shared_ptr<Object> LinkedList::eval_quote() const
{
    if (!(m_items.size() == 2)) return Nil::get();
    auto it = m_items.begin();
    auto op = *it;
    it++;
    auto obj = *it;
    if (!(op->type() == Type::Symbol)) return Nil::get();
    if (!(Symbol::name(op) == "quote")) return Nil::get();
    return obj;
}

std::shared_ptr<Object> LinkedList::eval_set(std::shared_ptr<Environment> env) const
{
    if (!(m_items.size() == 3)) return Nil::get();
    auto it = m_items.begin();
    auto op = *it;
    it++;
    auto key = *it;
    it++;
    auto body = *it;
    if (!(op->type() == Type::Symbol)) return Nil::get();
    if (!(Symbol::name(op) == "set!")) return Nil::get();
    return env->set(Symbol::name(key), lisp::eval(env, body));
}

std::shared_ptr<Object> LinkedList::eval_if(std::shared_ptr<Environment> env) const
{
    if (!(m_items.size() == 4)) return Nil::get();
    auto it = m_items.begin();
    auto op = *it;
    it++;
    auto pred = *it;
    it++;
    auto on_true = *it;
    it++;
    auto on_false = *it;
    if (!(op->type() == Type::Symbol)) return Nil::get();
    if (!(Symbol::name(op) == "if")) return Nil::get();
    if (Boolean::value(lisp::eval(env, pred)))
    {
        return lisp::eval(env, on_true);
    }
    else
    {
        return lisp::eval(env, on_false);
    }
}

std::shared_ptr<Object> LinkedList::eval_cond(std::shared_ptr<Environment> env) const
{
    if (!(m_items.size() >= 2)) return Nil::get();
    auto it = m_items.begin();
    auto op = *it;
    if (!(op->type() == Type::Symbol)) return Nil::get();
    if (!(Symbol::name(op) == "cond")) return Nil::get();
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
    if (!(m_items.size() == 3)) return Nil::get();
    auto it = m_items.begin();
    auto op = *it;
    it++;
    auto key = *it;
    it++;
    auto body = *it;
    if (!(op->type() == Type::Symbol)) return Nil::get();
    if (!(Symbol::name(op) == "def")) return Nil::get();
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
            if (fn->type() != Type::Symbol) return Nil::get();

            std::list<std::shared_ptr<Object>> params;
            for (it->next(); !it->is_done(); it->next())
            {
                params.push_back(it->get());
            }
            auto lambda = std::shared_ptr<Object>(new Lambda(std::shared_ptr<Object>(new LinkedList(params)), body, env));
            return env->define(Symbol::name(fn), lambda);
        }
        return Nil::get();
    }
    return Nil::get();
}

std::shared_ptr<Object> LinkedList::eval_lambda(std::shared_ptr<Environment> env) const
{
    if (!(m_items.size() == 3)) return Nil::get();
    auto it = m_items.begin();
    auto op = *it;
    it++;
    auto params = *it;
    it++;
    auto body = *it;
    if (!(op->type() == Type::Symbol)) return Nil::get();
    if (!(Symbol::name(op) == "lam")) return Nil::get();
    if (!(params->type() == Type::List)) return Nil::get();
    return std::shared_ptr<Object>(new Lambda(params, body, env));
}

std::shared_ptr<Object> LinkedList::eval_function(std::shared_ptr<Environment> env) const
{
    if (!(m_items.size() >= 1)) return Nil::get();
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
    if (!(m_items.size() == 3)) return Nil::get();
    auto it = m_items.begin();
    auto op = *it;
    it++;
    auto fst = *it;
    it++;
    auto snd = *it;
    if (!(op->type() == Type::Symbol)) return Nil::get();
    if (!(Symbol::name(op) == "cons")) return Nil::get();
    auto car = lisp::eval(env, fst);
    auto cdr = lisp::eval(env, snd);
    return lisp::cons(car, cdr);
}

std::shared_ptr<Object> LinkedList::eval_car(std::shared_ptr<Environment> env) const
{
    if (!(m_items.size() == 2)) return Nil::get();
    auto it = m_items.begin();
    auto op = *it;
    it++;
    auto list = *it;
    if (!(op->type() == Type::Symbol)) return Nil::get();
    if (!(Symbol::name(op) == "car")) return Nil::get();
    return List::to(lisp::eval(env, list))->car();
}

std::shared_ptr<Object> LinkedList::eval_cdr(std::shared_ptr<Environment> env) const
{
    if (!(m_items.size() == 2)) return Nil::get();
    auto it = m_items.begin();
    auto op = *it;
    it++;
    auto list = *it;
    if (!(op->type() == Type::Symbol)) return Nil::get();
    if (!(Symbol::name(op) == "cdr")) return Nil::get();
    return List::to(lisp::eval(env, list))->cdr();
}

std::shared_ptr<Object> LinkedList::eval_eq(std::shared_ptr<Environment> env) const
{
    if (!(m_items.size() == 3)) return Nil::get();
    auto it = m_items.begin();
    auto op = *it;
    it++;
    auto fst = *it;
    it++;
    auto snd = *it;
    if (!(op->type() == Type::Symbol)) return Nil::get();
    if (!(Symbol::name(op) == "eq?")) return Nil::get();
    auto left = lisp::eval(env, fst);
    auto right = lisp::eval(env, snd);
    return std::shared_ptr<Boolean>(new Boolean(left->eq(right)));
}

LinkedListIterator::LinkedListIterator(std::list<std::shared_ptr<Object>>::const_iterator start, std::list<std::shared_ptr<Object>>::const_iterator last)
    : m_current(start)
    , m_last(last)
    , m_end(last)
{
    m_last--;
}

void LinkedListIterator::next()
{
    m_current++;
}

bool LinkedListIterator::is_done() const
{
    return m_current == m_end;
}

bool LinkedListIterator::is_last() const
{
    return m_current == m_last;
}

std::shared_ptr<Object> LinkedListIterator::get() const
{
    return *m_current;
}

}
