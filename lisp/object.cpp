#include "object.h"
#include "error.h"
#include <string>
#include <sstream>

namespace lisp
{

void Object::display(std::ostream &os) const
{
    os << *this;
}

const std::string &Object::ud_type() const
{
    throw Error("not a user-defined type");
}

std::shared_ptr<Object> Object::eval(std::shared_ptr<Environment>)
{
    std::string msg = "can't eval: \'";
    std::ostringstream oss;
    oss << *this;
    msg += oss.str();
    msg += "\'";
    throw Error(msg);
}

std::shared_ptr<Object> Object::apply(std::shared_ptr<Object>)
{
    throw Error::with_object("can't apply: ", *this);
}

std::shared_ptr<Object> eval(std::shared_ptr<Environment> env, std::shared_ptr<Object> obj)
{
    auto type = obj->type();
    if (type != Type::Symbol && type != Type::List && type != Type::Sequence)
    {
        return obj;
    }
    return obj->eval(env);
}

std::ostream &operator <<(std::ostream &os, const Object &obj)
{
    obj.write(os);
}

std::ostream &operator <<(std::ostream &os, const std::shared_ptr<Object> obj)
{
    os << *obj;
}

}
