#ifndef REPL_H
#define REPL_H

#include "lisp.h"
#include "app.h"

class Repl : public App
{
public:
    Repl();
    Repl(std::shared_ptr<lisp::Environment> env);
    int exec();
private:
    std::shared_ptr<lisp::Environment> m_env;
};

#endif // REPL_H
