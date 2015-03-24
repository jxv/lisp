#ifndef APP_RUNNER_H
#define APP_RUNNER_H

#include "base.h"

namespace app
{

class SourceRunner : public Base
{
public:
    SourceRunner(const std::string &file_path);
    SourceRunner(std::shared_ptr<lisp::Environment> env, const std::string &file_path);
    int exec();
private:
    const std::string m_file_path;
    std::shared_ptr<lisp::Environment> m_env;
};

}

#endif // APP_RUNNER_H
