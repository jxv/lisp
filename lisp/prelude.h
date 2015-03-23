#ifndef LISP_PRELUDE_H
#define LISP_PRELUDE_H

namespace lisp
{
std::shared_ptr<lisp::Environment> prelude();
std::shared_ptr<lisp::Environment> static_prelude();

}

#endif // LISP_PRELUDE_H
