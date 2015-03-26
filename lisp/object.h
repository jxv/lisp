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
    Nil,
    List,
    Symbol,
    Boolean,
    Function,
    String,
    Sequence,
    Quote,
    UChar,
    Char,
    U16,
    U32,
    U64,
    I16,
    I32,
    I64,
    F32,
    F64,
    UD,
};

class Object
{
public:
    virtual void write(std::ostream &) const = 0;
    virtual void show(std::ostream &) const;
    virtual Type type() const = 0;
    virtual const std::string &ud_type() const;
    virtual std::shared_ptr<Object> eval(std::shared_ptr<Environment>);
    virtual std::shared_ptr<Object> apply(std::shared_ptr<Object>);
    virtual bool eq(std::shared_ptr<Object>) const = 0;
};

std::shared_ptr<Object> eval(std::shared_ptr<Environment> env, std::shared_ptr<Object> obj);
std::ostream &operator <<(std::ostream &, const Object &);
std::ostream &operator <<(std::ostream &, const std::shared_ptr<Object>);

}

#endif // LISP_OBJECT_H
