#include <unistd.h>
#include "app.h"
#include "repl.h"
#include "runner.h"

App::App(int argc, char * const argv[])
    : m_app(nullptr)
{
    if (argc == 1)
    {
        m_app = std::unique_ptr<app::Base>(new app::Repl);
    }
    else
    {
        int opt;
        while ((opt = getopt(argc, argv, "x:")) != -1)
        {
            if (opt == 'x')
            {
                m_app = std::unique_ptr<app::Base>(new app::SourceRunner(optarg));
                break;
            }
            else
            {
                break;
            }
        }
    }
}

int App::exec()
{
    if (m_app != nullptr)
    {
        return m_app->exec();
    }
    else
    {
        return EXIT_FAILURE;
    }
}
