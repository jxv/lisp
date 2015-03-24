#ifndef APP_APP_H
#define APP_APP_H

#include "base.h"

class App
{
public:
    App(int argc, char * const argv[]);
    int exec();
private:
    std::unique_ptr<app::Base> m_app;
};

#endif // APP_APP_H
