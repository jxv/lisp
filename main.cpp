#include <unistd.h>
#include "repl.h"
#include "runner.h"

int main(int argc, char * const argv[])
{
    std::unique_ptr<App> app = nullptr;
    if (argc == 1)
    {
        app = std::unique_ptr<App>(new Repl);
    }
    else
    {
        int opt;
        while ((opt = getopt(argc, argv, "x:")) != -1)
        {
            if (opt == 'x')
            {
                app = std::unique_ptr<App>(new SourceRunner(optarg));
                break;
            }
            else
            {
                break;
            }
        }
    }
    if (app != nullptr)
    {
        app->exec();
        return EXIT_SUCCESS;
    }
    else
    {
        return EXIT_FAILURE;
    }
}
