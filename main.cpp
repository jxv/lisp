#include "lisp.h"

std::shared_ptr<lisp::Object> display(std::shared_ptr<lisp::Object> obj)
{
    auto args = lisp::List::to(obj);
    if (args->size() != 1)
    {
        throw lisp::Error::with_object("display: needs 1 arg: ", *obj);
    }
    args->car()->display(std::cout);
    return lisp::Void::get();
}

std::shared_ptr<lisp::Object> add(std::shared_ptr<lisp::Object> obj)
{
    auto args = lisp::List::to(obj);
    if (args->size() < 2)
    {
        throw lisp::Error::with_object("+: needs 2 or more args: ", *obj);
    }
    auto it = args->iterator();
    switch (it->get()->type())
    {
    case lisp::Type::F32:
    {
        float result = 0;
        for (; !it->is_done(); it->next())
        {
            result += lisp::F32::value(it->get());
        }
        return std::shared_ptr<lisp::Object>(new lisp::F32(result));
    }
    case lisp::Type::I32:
    default:
    {
        int result = 0;
        for (; !it->is_done(); it->next())
        {
            result += lisp::I32::value(it->get());
        }
        return std::shared_ptr<lisp::Object>(new lisp::I32(result));
    }
    }
}

std::shared_ptr<lisp::Object> multiply(std::shared_ptr<lisp::Object> obj)
{
    auto args = lisp::List::to(obj);
    if (args->size() < 2)
    {
        throw lisp::Error::with_object("*: needs 2 or more args: ", *obj);
    }
    int result = 1;
    for (auto it = args->iterator(); !it->is_done(); it->next())
    {
        result *= lisp::I32::value(it->get());
    }
    return std::shared_ptr<lisp::Object>(new lisp::I32(result));
}

std::shared_ptr<lisp::Object> subtract(std::shared_ptr<lisp::Object> obj)
{
    auto args = lisp::List::to(obj);
    if (args->size() < 2)
    {
        throw lisp::Error::with_object("-: needs 2 or more args: ", *obj);
    }

    auto it = args->iterator();
    int result = lisp::I32::value(it->get());
    it->next();
    for (; !it->is_done(); it->next())
    {
        result -= lisp::I32::value(it->get());
    }
    return std::shared_ptr<lisp::Object>(new lisp::I32(result));
}

std::shared_ptr<lisp::Object> divide(std::shared_ptr<lisp::Object> obj)
{
    auto args = lisp::List::to(obj);
    if (args->size() < 2)
    {
        throw lisp::Error::with_object("/: needs 2 or more args: ", *obj);
    }

    auto it = args->iterator();
    int result = lisp::I32::value(it->get());
    it->next();
    for (; !it->is_done(); it->next())
    {
        result /= lisp::I32::value(it->get());
    }
    return std::shared_ptr<lisp::Object>(new lisp::I32(result));
}

std::shared_ptr<lisp::Object> equal_to(std::shared_ptr<lisp::Object> obj)
{
    auto args = lisp::List::to(obj);
    if (args->size() != 2)
    {
        throw lisp::Error::with_object("=: needs 2 args: ", *obj);
    }

    auto it = args->iterator();
    auto first = it->get();
    it->next();
    auto result = lisp::I32::value(first) == lisp::I32::value(it->get());
    return std::shared_ptr<lisp::Object>(new lisp::Boolean(result));
}

std::shared_ptr<lisp::Object> greater_than(std::shared_ptr<lisp::Object> obj)
{
    auto args = lisp::List::to(obj);
    if (args->size() != 2)
    {
        throw lisp::Error::with_object(">: needs 2 args: ", *obj);
    }

    auto it = args->iterator();
    auto first = it->get();
    it->next();
    auto result = lisp::I32::value(first) > lisp::I32::value(it->get());
    return std::shared_ptr<lisp::Object>(new lisp::Boolean(result));
}

std::shared_ptr<lisp::Object> greater_than_or_equal(std::shared_ptr<lisp::Object> obj)
{
    auto args = lisp::List::to(obj);
    if (args->size() != 2)
    {
        throw lisp::Error::with_object(">=: needs 2 args: ", *obj);
    }

    auto it = args->iterator();
    auto first = it->get();
    it->next();
    auto result = lisp::I32::value(first) >= lisp::I32::value(it->get());
    return std::shared_ptr<lisp::Object>(new lisp::Boolean(result));
}

std::shared_ptr<lisp::Object> less_than(std::shared_ptr<lisp::Object> obj)
{
    auto args = lisp::List::to(obj);
    if (args->size() != 2)
    {
        throw lisp::Error::with_object("<: needs 2 args: ", *obj);
    }

    auto it = args->iterator();
    auto first = it->get();
    it->next();
    auto result = lisp::I32::value(first) < lisp::I32::value(it->get());
    return std::shared_ptr<lisp::Object>(new lisp::Boolean(result));
}

std::shared_ptr<lisp::Object> less_than_or_equal(std::shared_ptr<lisp::Object> obj)
{
    auto args = lisp::List::to(obj);
    if (args->size() != 2)
    {
        throw lisp::Error::with_object("<=: needs 2 args: ", *obj);
    }

    auto it = args->iterator();
    auto first = it->get();
    it->next();
    auto result = lisp::I32::value(first) <= lisp::I32::value(it->get());
    return std::shared_ptr<lisp::Object>(new lisp::Boolean(result));
}

std::shared_ptr<lisp::Object> round_i32(std::shared_ptr<lisp::Object> obj)
{
    auto args = lisp::List::to(obj);
    if (args->size() != 1)
    {
        throw lisp::Error::with_object("round: needs 1 arg: ", *args->car());
    }
    auto result = roundf(lisp::F32::value(args->car()));
    return std::shared_ptr<lisp::Object>(new lisp::I32(result));
}

void def_cpp_fn(
    std::shared_ptr<lisp::Environment> env,
    const std::string &name, std::shared_ptr<lisp::Object> (*fn)(std::shared_ptr<lisp::Object> obj)
){
    env->define(name, std::shared_ptr<lisp::Object>(new lisp::CppFunction(fn, name)));
}

namespace lisp
{

std::shared_ptr<lisp::Environment> prelude()
{
    auto env = std::shared_ptr<lisp::Environment>(new lisp::Environment);
    def_cpp_fn(env, "display", display);
    def_cpp_fn(env, "+", add);
    def_cpp_fn(env, "*", multiply);
    def_cpp_fn(env, "-", subtract);
    def_cpp_fn(env, "/", divide);
    def_cpp_fn(env, "=", equal_to);
    def_cpp_fn(env, ">", greater_than);
    def_cpp_fn(env, ">=", greater_than_or_equal);
    def_cpp_fn(env, "<", less_than);
    def_cpp_fn(env, "<=", less_than_or_equal);
    def_cpp_fn(env, "round", round_i32);
    return env;
}

}

int main(int, const char **)
{
    auto env = lisp::prelude();

    // repl
    for (;;)
    {
        std::cin.clear();
        std::cout << "\u03bb . ";
        std::string input;
        std::getline(std::cin, input);

        // ignore lines without code
        {
            if (input.length() == 0)
            {
                continue;
            }
            bool non_spacing = false;
            for (auto it = input.begin(); it != input.end(); ++it)
            {
                if (*it != ' ' && *it != '\t')
                {
                    non_spacing = true;
                    break;
                }
            }
            if (!non_spacing)
            {
                continue;
            }
        }

        // eval parsed
        try
        {
            auto parser = lisp::Parser(input);
            auto result = lisp::eval(env, parser.parse());
            if (result != nullptr && result != lisp::Void::get())
            {
                std::cout << result << std::endl;
            }
        }
        catch (const lisp::Error &e)
        {
            std::cout << e.message() << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
