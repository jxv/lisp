#ifndef LISP_LINKEDLIST_H
#define LISP_LINKEDLIST_H

#include "list.h"

namespace lisp
{

class LinkedList : public List
{
public:
    LinkedList(std::list<std::shared_ptr<Object>> &items);
    std::shared_ptr<Object> car() const;
    std::shared_ptr<Object> cdr() const;
    std::shared_ptr<Object> eval(std::shared_ptr<Environment> env);
    unsigned int size() const;
    std::unique_ptr<lisp::Iterator> iterator() const;
private:
    std::shared_ptr<Object> eval_quote() const;
    std::shared_ptr<Object> eval_set(std::shared_ptr<Environment> env) const;
    std::shared_ptr<Object> eval_if(std::shared_ptr<Environment> env) const;
    std::shared_ptr<Object> eval_cond(std::shared_ptr<Environment> env) const;
    std::shared_ptr<Object> eval_define(std::shared_ptr<Environment> env) const;
    std::shared_ptr<Object> eval_lambda(std::shared_ptr<Environment> env) const;
    std::shared_ptr<Object> eval_cons(std::shared_ptr<Environment> env) const;
    std::shared_ptr<Object> eval_car(std::shared_ptr<Environment> env) const;
    std::shared_ptr<Object> eval_cdr(std::shared_ptr<Environment> env) const;
    std::shared_ptr<Object> eval_eq(std::shared_ptr<Environment> env) const;
    std::shared_ptr<Object> eval_function(std::shared_ptr<Environment> env) const;
private:
    std::list<std::shared_ptr<Object>> m_items;
};

class LinkedListIterator : public Iterator
{
public:
    LinkedListIterator(std::list<std::shared_ptr<Object>>::const_iterator current, std::list<std::shared_ptr<Object>>::const_iterator last);
    void next();
    bool is_done() const;
    bool is_last() const;
    std::shared_ptr<lisp::Object> get() const;
private:
    std::list<std::shared_ptr<Object>>::const_iterator m_current;
    std::list<std::shared_ptr<Object>>::const_iterator m_last;
    std::list<std::shared_ptr<Object>>::const_iterator m_end;
};


}

#endif // LISP_LINKEDLIST_H
