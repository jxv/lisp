#ifndef LISP_SEQUENCE_H
#define LISP_SEQUENCE_H

#include "object.h"

namespace lisp
{

class Sequence : public Object
{
public:
    Sequence(std::vector<std::shared_ptr<Object>> &statements);
    void write(std::ostream &) const;
    bool eq(std::shared_ptr<Object>) const;
    Type type() const;
    std::shared_ptr<Object> eval(std::shared_ptr<Environment> env);
    static std::shared_ptr<Sequence> to(std::shared_ptr<Object>);
private:
    std::vector<std::shared_ptr<Object>> m_statements;
};

}

#endif
