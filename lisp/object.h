#ifndef LISP_OBJECT_H
#define LISP_OBJECT_H

#include <iostream>
#include <list>
#include <map>
#include <string>
#include <memory>
#include <exception>
#include <algorithm>
#include <vector>

namespace lisp
{

class Environment;

enum class Type
{
    Void,
    List,
    Symbol,
    Boolean,
    Function,
    String,
    Sequence,
    I32,
    F32,
};

class Object
{
public:
    virtual void write(std::ostream &) const = 0;
    virtual void display(std::ostream &) const;
    virtual Type type() const = 0;
    virtual std::shared_ptr<Object> eval(std::shared_ptr<Environment>);
    virtual std::shared_ptr<Object> apply(std::shared_ptr<Object>);
};

std::shared_ptr<Object> eval(std::shared_ptr<Environment> env, std::shared_ptr<Object> obj);
std::ostream &operator <<(std::ostream &, const Object &);
std::ostream &operator <<(std::ostream &, const std::shared_ptr<Object>);

}

#endif // LISP_OBJECT_H
