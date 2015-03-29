#include "object.h"
#include "error.h"
#include <string>
#include <sstream>

using std::shared_ptr;

namespace lisp
{

void Object::show(std::ostream &os) const
{
    os << *this;
}

const std::string &Object::ud_type() const
{
    throw Error("not a user-defined type");
}

shared_ptr<Object> Object::eval(shared_ptr<Environment>)
{
    std::string msg = "can't eval: \'";
    std::ostringstream oss;
    oss << *this;
    msg += oss.str();
    msg += "\'";
    throw Error(msg);
}

shared_ptr<Object> Object::apply(shared_ptr<Object>)
{
    throw Error::with_object("can't apply: ", *this);
}

shared_ptr<Object> eval(shared_ptr<Environment> env, shared_ptr<Object> obj)
{
    auto type = obj->type();
    if (type != Type::Symbol && type != Type::List && type != Type::Sequence && type != Type::Quote)
    {
        return obj;
    }
    return obj->eval(env);
}

std::ostream &operator <<(std::ostream &os, const Object &obj)
{
    obj.write(os);
}

std::ostream &operator <<(std::ostream &os, const shared_ptr<Object> obj)
{
    os << *obj;
}

}
