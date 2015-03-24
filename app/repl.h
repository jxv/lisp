#ifndef APP_REPL_H
#define APP_REPL_H

#include "base.h"

namespace app
{

class Repl : public Base
{
public:
    Repl();
    Repl(std::shared_ptr<lisp::Environment> env);
    int exec();
private:
    std::shared_ptr<lisp::Environment> m_env;
};

}

#endif // APP_REPL_H
