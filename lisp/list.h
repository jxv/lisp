#ifndef LISP_LIST_H
#define LISP_LIST_H

#include "object.h"

namespace lisp
{

class Iterator;

class List : public Object
{
public:
    virtual void write(std::ostream &os) const;
    Type type() const;
    virtual std::shared_ptr<Object> car() const = 0;
    virtual std::shared_ptr<Object> cdr() const = 0;
    virtual std::shared_ptr<Object> eval(std::shared_ptr<Environment> env) = 0;
    virtual unsigned int size() const = 0;
    virtual std::unique_ptr<lisp::Iterator> iterator() const = 0;
    virtual bool is_last_empty() const = 0;
    virtual void set_last_empty() = 0;
    static std::shared_ptr<List> to(std::shared_ptr<Object>);
};

class Iterator
{
public:
    virtual void next() = 0;
    virtual bool is_done() const = 0;
    virtual bool is_last() const = 0;
    virtual std::shared_ptr<lisp::Object> get() const = 0;
};

class Empty : public List
{
public:
    void write(std::ostream &os) const;
    std::shared_ptr<Object> car() const;
    std::shared_ptr<Object> cdr() const;
    std::shared_ptr<Object> eval(std::shared_ptr<Environment> env);
    unsigned int size() const;
    std::unique_ptr<lisp::Iterator> iterator() const;
    bool is_last_empty() const;
    void set_last_empty();
    static std::shared_ptr<Object> get();
private:
    Empty();
};


class LinkedList : public List
{
public:
    LinkedList(std::list<std::shared_ptr<Object>> &items, bool is_last_empty = false);
    std::shared_ptr<Object> car() const;
    std::shared_ptr<Object> cdr() const;
    std::shared_ptr<Object> eval(std::shared_ptr<Environment> env);
    unsigned int size() const;
    std::unique_ptr<lisp::Iterator> iterator() const;
    bool is_last_empty() const;
    void set_last_empty();
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
    std::shared_ptr<Object> eval_function(std::shared_ptr<Environment> env) const;
private:
    bool m_is_last_empty;
    std::list<std::shared_ptr<Object>> m_items;
};

class LinkedList_Iterator : public Iterator
{
public:
    LinkedList_Iterator(std::list<std::shared_ptr<Object>>::const_iterator current, std::list<std::shared_ptr<Object>>::const_iterator last);
    void next();
    bool is_done() const;
    bool is_last() const;
    std::shared_ptr<lisp::Object> get() const;
private:
    std::list<std::shared_ptr<Object>>::const_iterator m_current;
    std::list<std::shared_ptr<Object>>::const_iterator m_last;
    std::list<std::shared_ptr<Object>>::const_iterator m_end;
};

std::shared_ptr<Object> cons(std::shared_ptr<Object> car, std::shared_ptr<Object> cdr);

}

#endif // LISP_LIST_H
