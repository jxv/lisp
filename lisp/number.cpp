#include "number.h"
#include "error.h"

namespace lisp
{

I32::I32(int v)
    : Number(v)
{
}

bool I32::eq(std::shared_ptr<Object> obj) const
{
    try
    {
        return Number::value() == I32::value(obj);
    }
    catch(lisp::Error)
    {
    }
    return false;
}

std::shared_ptr<I32> I32::to(std::shared_ptr<Object> obj)
{
    if (obj->type() != Type::I32)
    {
        throw Error::with_object("not i32", *obj);
    }
    return std::dynamic_pointer_cast<I32>(obj);
}

int I32::value(std::shared_ptr<Object> obj)
{
    return std::static_pointer_cast<Number>(I32::to(obj))->value();
}

F32::F32(float v)
    : Number(v)
{
}

bool F32::eq(std::shared_ptr<Object> obj) const
{
    try
    {
        return Number::value() == F32::value(obj);
    }
    catch(lisp::Error)
    {
    }
    return false;
}

std::shared_ptr<F32> F32::to(std::shared_ptr<Object> obj)
{
    if (obj->type() != Type::F32)
    {
        throw Error::with_object("not f32", *obj);
    }
    return std::dynamic_pointer_cast<F32>(obj);
}

float F32::value(std::shared_ptr<Object> obj)
{
    return std::static_pointer_cast<Number>(F32::to(obj))->value();
}

}
