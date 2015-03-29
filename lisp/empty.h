#ifndef LISP_EMPTY_H
#define LISP_EMPTY_H

#include "list.h"

namespace lisp
{

class Empty : public List
{
public:
    void write(std::ostream &os) const;
    std::shared_ptr<Object> car() const;
    std::shared_ptr<Object> cdr() const;
    std::shared_ptr<Object> eval(std::shared_ptr<Environment> env);
    unsigned int size() const;
    std::unique_ptr<lisp::Iterator> iterator() const;
    static std::shared_ptr<Object> get();
private:
    Empty();
};

class EmptyIterator : public Iterator
{
public:
    EmptyIterator();
    void next();
    bool is_done() const;
    bool is_last() const;
    std::shared_ptr<Object> get() const;
};

}

#endif // LISP_EMPTY_H
