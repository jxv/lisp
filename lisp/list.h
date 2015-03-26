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
    bool eq(std::shared_ptr<Object>) const;
    virtual std::shared_ptr<Object> car() const = 0;
    virtual std::shared_ptr<Object> cdr() const = 0;
    virtual std::shared_ptr<Object> eval(std::shared_ptr<Environment> env) = 0;
    virtual unsigned int size() const = 0;
    virtual std::unique_ptr<lisp::Iterator> iterator() const = 0;
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

std::shared_ptr<Object> cons(std::shared_ptr<Object> car, std::shared_ptr<Object> cdr);

}

#endif // LISP_LIST_H
