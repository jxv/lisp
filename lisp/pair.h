#ifndef LISP_PAIR_H
#define LISP_PAIR_H

#include "list.h"

namespace lisp
{

class Pair : public List
{
public:
    Pair(std::shared_ptr<Object> car, std::shared_ptr<Object> cdr);
    std::shared_ptr<Object> car() const;
    std::shared_ptr<Object> cdr() const;
    std::shared_ptr<Object> eval(std::shared_ptr<Environment> env);
    unsigned int size() const;
    std::unique_ptr<lisp::Iterator> iterator() const;
private:
    std::shared_ptr<Object> m_car;
    std::shared_ptr<Object> m_cdr; 
};

class PairIterator : public Iterator
{
public:
    PairIterator(std::shared_ptr<Object> car, std::shared_ptr<Object> cdr);
    void next();
    bool is_done() const;
    bool is_last() const;
    std::shared_ptr<lisp::Object> get() const;
private:
    std::shared_ptr<Object> m_car;
    std::shared_ptr<Object> m_cdr; 
};

}

#endif // LISP_PAIR_H
