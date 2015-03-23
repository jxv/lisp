#ifndef LISP_ENVIRONMENT_H
#define LISP_ENVIRONMENT_H

#include "object.h"

namespace lisp
{

class Environment
{
public:
    Environment();
    Environment(std::shared_ptr<Environment> parent);
    std::shared_ptr<Environment> m_parent;
    std::shared_ptr<Object> get(const std::string &) const;
    std::shared_ptr<Object> set(const std::string &, std::shared_ptr<Object>);
    std::shared_ptr<Object> define(const std::string &, std::shared_ptr<Object>);
    std::shared_ptr<Object> &operator[] (const std::string &);
private:
    std::map<std::string, std::shared_ptr<Object>> m_map;
};

}

#endif // LISP_ENVIRONMENT_H
