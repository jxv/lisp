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
    static std::shared_ptr<Void> get();
};

}

#endif // LISP_VOID_H
