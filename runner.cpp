#include "runner.h"
#include <vector>
#include <fstream>
#include <streambuf>

SourceRunner::SourceRunner(const std::string &file_path)
    : SourceRunner(lisp::static_prelude(), file_path)
{
}

SourceRunner::SourceRunner(std::shared_ptr<lisp::Environment> env, const std::string &file_path)
    : m_file_path(file_path)
    , m_env(std::shared_ptr<lisp::Environment>(new lisp::Environment(env)))
{
}

int SourceRunner::exec()
{
    std::string input;
    try
    {
        std::ifstream file(m_file_path.c_str());
        input = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    }
    catch (std::exception)
    {
        std::cout << "can't read file '" << m_file_path << "'." << std::endl;
        return EXIT_FAILURE;
    }
    try
    {
        auto parser = lisp::Parser(input);
        lisp::eval(m_env, parser.parse());
    }
    catch (const lisp::Error &e)
    {
        std::cout << e.message() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
