#ifndef LISP_INTEGER_H
#define LISP_INTEGER_H

#include "object.h"

namespace lisp
{

template <typename A, Type B>
class Number : public Object
{
public:
    Number(A value);
    void write(std::ostream &os) const;
    virtual bool eq(std::shared_ptr<Object>) const = 0;
    Type type() const;
    A value() const;
private:
    A m_value;
};

template <typename A, Type B>    
Number<A,B>::Number(A value)
    : m_value(value)
{
}

template <typename A, Type B>    
void Number<A,B>::write(std::ostream &os) const
{
    os << m_value;
}

template <typename A, Type B>    
Type Number<A,B>::type() const
{
    return B;
}


template <typename A, Type B>
A Number<A,B>::value() const
{
    return m_value;
}

class I32 : public Number<int, Type::I32>
{
public:
    I32(int v);
    bool eq(std::shared_ptr<Object>) const;
    static std::shared_ptr<I32> to(std::shared_ptr<Object>);
    static int value(std::shared_ptr<Object>);
};

class F32 : public Number<float, Type::F32>
{
public:
    F32(float v);
    bool eq(std::shared_ptr<Object>) const;
    static std::shared_ptr<F32> to(std::shared_ptr<Object>);
    static float value(std::shared_ptr<Object>);
};

}

#endif
