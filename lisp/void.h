#ifndef LISP_VOID_H
#define LISP_VOID_H

#include "object.h"

namespace lisp
{

class Void : public Object
{
public:
    void write(std::ostream &) const;
    Type type() const;
    bool eq(std::shared_ptr<Object> obj) const;
    static std::shared_ptr<Void> get();
};

}

#endif // LISP_VOID_H
