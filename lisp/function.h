#ifndef LISP_FUNCTION_H
#define LISP_FUNCTION_H

#include <functional>
#include "object.h"

namespace lisp
{

class Function : public Object
{
public:
    void write(std::ostream &os) const = 0;
    virtual bool eq(std::shared_ptr<Object>) const = 0;
    Type type() const;
    virtual std::shared_ptr<Object> apply(std::shared_ptr<Object>) = 0;
};

class CppFunction : public Function, public std::function<std::shared_ptr<Object>(std::shared_ptr<Object>)>
{
public:
    CppFunction(std::shared_ptr<Object> (*)(std::shared_ptr<Object>));
    CppFunction(std::shared_ptr<Object> (*)(std::shared_ptr<Object>), const std::string &name);
    void write(std::ostream &os) const;
    bool eq(std::shared_ptr<Object>) const;
    std::shared_ptr<Object> apply(std::shared_ptr<Object>);
    CppFunction &operator =(const CppFunction &);
    CppFunction &operator =(const std::function<std::shared_ptr<Object>(std::shared_ptr<Object>)> &);
private:
    const std::string m_name;
};

class Lambda : public Function
{
public:
    Lambda(std::shared_ptr<Object> params, std::shared_ptr<Object> body, std::shared_ptr<Environment> env);
    void write(std::ostream &os) const;
    bool eq(std::shared_ptr<Object>) const;
    std::shared_ptr<Object> apply(std::shared_ptr<Object>);
private:
    std::vector<std::string> m_params;
    std::shared_ptr<Object> m_body;
    std::shared_ptr<Environment> m_env_parent;
};

}

#endif // LISP_FUNCTION_H
