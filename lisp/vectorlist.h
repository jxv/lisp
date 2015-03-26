#ifndef LISP_VECTORLIST_H
#define LISP_VECTORLIST_H

#include "list.h"

namespace lisp
{

class VectorList : public List
{
public:
    VectorList(std::vector<std::shared_ptr<Object>> &items);
    std::shared_ptr<Object> car() const;
    std::shared_ptr<Object> cdr() const;
    std::shared_ptr<Object> eval(std::shared_ptr<Environment> env);
    unsigned int size() const;
    std::unique_ptr<Iterator> iterator() const;
private:
    std::vector<std::shared_ptr<Object>> m_items;
};

class VectorListIterator : public Iterator
{
public:
    VectorListIterator(std::vector<std::shared_ptr<Object>>::const_reverse_iterator current, std::vector<std::shared_ptr<Object>>::const_reverse_iterator last);
    void next();
    bool is_done() const;
    bool is_last() const;
    std::shared_ptr<lisp::Object> get() const;
private:
    std::vector<std::shared_ptr<Object>>::const_reverse_iterator m_current;
    std::vector<std::shared_ptr<Object>>::const_reverse_iterator m_last;
    std::vector<std::shared_ptr<Object>>::const_reverse_iterator m_end;
};

}

#endif // LISP_VECTORLIST_H
