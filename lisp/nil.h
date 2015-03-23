#ifndef LISP_NIL_H
#define LISP_NIL_H

#include "object.h"

namespace lisp
{

class Nil : public Object
{
public:
    void write(std::ostream &) const;
    Type type() const;
    static std::shared_ptr<Nil> get();
private:
    Nil();
};

}

#endif // LISP_NIL_H
