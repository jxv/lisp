#ifndef RUNNER_H
#define RUNNER_H

#include "lisp.h"
#include "app.h"

class SourceRunner : public App
{
public:
    SourceRunner(const std::string &file_path);
    SourceRunner(std::shared_ptr<lisp::Environment> env, const std::string &file_path);
    int exec();
private:
    const std::string m_file_path;
    std::shared_ptr<lisp::Environment> m_env;
};

#endif
