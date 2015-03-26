#include "boolean.h"
#include "empty.h"
#include "environment.h"
#include "error.h"
#include "number.h"
#include "function.h"
#include "list.h"
#include "object.h"
#include "sequence.h"
#include "string.h"
#include "symbol.h"
#include "void.h"

using std::shared_ptr;

namespace lisp
{

shared_ptr<Object> is_null(shared_ptr<Object> obj)
{
    auto arg = List::to(obj)->car();
    return shared_ptr<Object>(new Boolean(arg == Empty::get()));
}

shared_ptr<Object> is_pair(shared_ptr<Object> obj)
{
    auto arg = List::to(obj)->car();
    return shared_ptr<Object>(new Boolean(arg->type() == Type::List));
}

shared_ptr<Object> print(shared_ptr<Object> obj)
{
    auto args = List::to(obj);
    if (args->size() != 1)
    {
        throw Error::with_object("print: needs 1 arg: ", *obj);
    }
    args->car()->show(std::cout);
    return Void::get();
}

shared_ptr<Object> get_line(shared_ptr<Object> obj)
{
    if (List::to(obj)->size() != 0)
    {
        throw Error("get-line: doesn't take any args");
    }
    std::string line;
    std::getline(std::cin, line);
    return shared_ptr<Object>(String::from_escape_chars(line));
}

shared_ptr<Object> add(shared_ptr<Object> obj)
{
    auto args = List::to(obj);
    if (args->size() < 2)
    {
        throw Error::with_object("+: needs 2 or more args: ", *obj);
    }
    auto it = args->iterator();
    switch (it->get()->type())
    {
    case Type::F32:
    {
        float result = 0;
        for (; !it->is_done(); it->next())
        {
            result += F32::value(it->get());
        }
        return shared_ptr<Object>(new F32(result));
    }
    case Type::I32:
    default:
    {
        int result = 0;
        for (; !it->is_done(); it->next())
        {
            result += I32::value(it->get());
        }
        return shared_ptr<Object>(new I32(result));
    }
    }
}

shared_ptr<Object> multiply(shared_ptr<Object> obj)
{
    auto args = List::to(obj);
    if (args->size() < 2)
    {
        throw Error::with_object("*: needs 2 or more args: ", *obj);
    }
    int result = 1;
    for (auto it = args->iterator(); !it->is_done(); it->next())
    {
        result *= I32::value(it->get());
    }
    return shared_ptr<Object>(new I32(result));
}

shared_ptr<Object> subtract(shared_ptr<Object> obj)
{
    auto args = List::to(obj);
    if (args->size() < 2)
    {
        throw Error::with_object("-: needs 2 or more args: ", *obj);
    }

    auto it = args->iterator();
    int result = I32::value(it->get());
    it->next();
    for (; !it->is_done(); it->next())
    {
        result -= I32::value(it->get());
    }
    return shared_ptr<Object>(new I32(result));
}

shared_ptr<Object> divide(shared_ptr<Object> obj)
{
    auto args = List::to(obj);
    if (args->size() < 2)
    {
        throw Error::with_object("/: needs 2 or more args: ", *obj);
    }

    auto it = args->iterator();
    int result = I32::value(it->get());
    it->next();
    for (; !it->is_done(); it->next())
    {
        result /= I32::value(it->get());
    }
    return shared_ptr<Object>(new I32(result));
}

shared_ptr<Object> equal_to(shared_ptr<Object> obj)
{
    auto args = List::to(obj);
    if (args->size() != 2)
    {
        throw Error::with_object("=: needs 2 args: ", *obj);
    }

    auto it = args->iterator();
    auto first = it->get();
    it->next();
    auto result = I32::value(first) == I32::value(it->get());
    return shared_ptr<Object>(new Boolean(result));
}

shared_ptr<Object> greater_than(shared_ptr<Object> obj)
{
    auto args = List::to(obj);
    if (args->size() != 2)
    {
        throw Error::with_object(">: needs 2 args: ", *obj);
    }

    auto it = args->iterator();
    auto first = it->get();
    it->next();
    auto result = I32::value(first) > I32::value(it->get());
    return shared_ptr<Object>(new Boolean(result));
}

shared_ptr<Object> greater_than_or_equal(shared_ptr<Object> obj)
{
    auto args = List::to(obj);
    if (args->size() != 2)
    {
        throw Error::with_object(">=: needs 2 args: ", *obj);
    }

    auto it = args->iterator();
    auto first = it->get();
    it->next();
    auto result = I32::value(first) >= I32::value(it->get());
    return shared_ptr<Object>(new Boolean(result));
}

shared_ptr<Object> less_than(shared_ptr<Object> obj)
{
    auto args = List::to(obj);
    if (args->size() != 2)
    {
        throw Error::with_object("<: needs 2 args: ", *obj);
    }

    auto it = args->iterator();
    auto first = it->get();
    it->next();
    auto result = I32::value(first) < I32::value(it->get());
    return shared_ptr<Object>(new Boolean(result));
}

shared_ptr<Object> less_than_or_equal(shared_ptr<Object> obj)
{
    auto args = List::to(obj);
    if (args->size() != 2)
    {
        throw Error::with_object("<=: needs 2 args: ", *obj);
    }

    auto it = args->iterator();
    auto first = it->get();
    it->next();
    auto result = I32::value(first) <= I32::value(it->get());
    return shared_ptr<Object>(new Boolean(result));
}

shared_ptr<Object> round_i32(shared_ptr<Object> obj)
{
    auto args = List::to(obj);
    if (args->size() != 1)
    {
        throw Error::with_object("round: needs 1 arg: ", *args->car());
    }
    auto result = roundf(F32::value(args->car()));
    return shared_ptr<Object>(new I32(result));
}

shared_ptr<Object> not_(shared_ptr<Object> obj)
{
    auto args = List::to(obj);
    if (args->size() != 1)
    {
        throw Error::with_object("not: needs 1 arg: ", *args->car());
    }
    return shared_ptr<Object>(new Boolean(!Boolean::value(args->car())));
}

shared_ptr<Object> and_(shared_ptr<Object> obj)
{
    auto args = List::to(obj);
    if (args->size() < 1)
    {
        throw Error::with_object("and: needs 2 or more args: ", *obj);
    }
    auto it = args->iterator();
    for (; !it->is_done(); it->next())
    {
        if (!Boolean::value(it->get()))
        {
            return shared_ptr<Object>(new Boolean(false));
        }
    }
    return shared_ptr<Object>(new Boolean(true));
}

shared_ptr<Object> or_(shared_ptr<Object> obj)
{
    auto args = List::to(obj);
    if (args->size() < 1)
    {
        throw Error::with_object("or: needs 2 or more args: ", *obj);
    }
    auto it = args->iterator();
    for (; !it->is_done(); it->next())
    {
        if (Boolean::value(it->get()))
        {
            return shared_ptr<Object>(new Boolean(true));
        }
    }
    return shared_ptr<Object>(new Boolean(false));
}

void def_cpp_fn(
    shared_ptr<Environment> env,
    const std::string &name, shared_ptr<Object> (*fn)(shared_ptr<Object> obj)
){
    env->define(name, shared_ptr<Object>(new CppFunction(fn, name)));
}

shared_ptr<Environment> prelude()
{
    auto env = shared_ptr<Environment>(new Environment);
    env->define("else", shared_ptr<Object>(new Boolean(true)));
    def_cpp_fn(env, "null?", is_null);
    def_cpp_fn(env, "pair?", is_pair);
    def_cpp_fn(env, "print", print);
    def_cpp_fn(env, "get-line", get_line);
    def_cpp_fn(env, "+", add);
    def_cpp_fn(env, "*", multiply);
    def_cpp_fn(env, "-", subtract);
    def_cpp_fn(env, "/", divide);
    def_cpp_fn(env, "=", equal_to);
    def_cpp_fn(env, ">", greater_than);
    def_cpp_fn(env, ">=", greater_than_or_equal);
    def_cpp_fn(env, "<", less_than);
    def_cpp_fn(env, "<=", less_than_or_equal);
    def_cpp_fn(env, "not", not_);
    def_cpp_fn(env, "and", and_);
    def_cpp_fn(env, "or", or_);
    def_cpp_fn(env, "round", round_i32);
    return env;
}

shared_ptr<Environment> static_prelude()
{
    static auto env = prelude();
    return env;
}

}
