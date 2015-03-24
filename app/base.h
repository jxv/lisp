#ifndef APP_BASE_H
#define APP_BASE_H

#include "../lisp.h"

namespace app
{

class Base
{
public:
    virtual int exec() = 0;
};

}

#endif // APP_BASE_H
