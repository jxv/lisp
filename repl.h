#ifndef REPL_H
#define REPL_H

#include "lisp.h"

class Repl
{
public:
    Repl();
    Repl(std::shared_ptr<lisp::Environment> env);
    void loop();
private:
    std::shared_ptr<lisp::Environment> m_env;
};

#endif // REPL_H
