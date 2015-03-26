#ifndef LISP_QUOTE
#define LISP_QUOTE

#include "object.h"

namespace lisp
{

class Quote : public Object
{
public:
    Quote(std::shared_ptr<Object> obj);
    void write(std::ostream &) const;
    bool eq(std::shared_ptr<Object> obj) const;
    void show(std::ostream &) const;
    Type type() const;
    std::shared_ptr<Object> eval(std::shared_ptr<Environment>);
    static std::shared_ptr<Quote> to(std::shared_ptr<Object>);
private:
    std::shared_ptr<Object> m_object;
};

}

#endif // LISP_QUOTE
