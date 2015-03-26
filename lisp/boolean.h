#ifndef LISP_BOOLEAN_H
#define LISP_BOOLEAN_H

#include "object.h"

namespace lisp
{

class Boolean : public Object
{
public:
    Boolean(bool v);
    Boolean();
    void write(std::ostream &os) const;
    Type type() const;
    bool eq(std::shared_ptr<Object>) const;
    bool value() const;
    static bool value(std::shared_ptr<Object>);
    static std::shared_ptr<Boolean> to(std::shared_ptr<Object>);
private:
    const bool m_value;
};

}

#endif // LISP_BOOLEAN_H
