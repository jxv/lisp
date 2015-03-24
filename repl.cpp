#include "repl.h"

Repl::Repl()
    : m_env(lisp::prelude())
{
}

Repl::Repl(std::shared_ptr<lisp::Environment> env)
    : m_env(env)
{
}

int Repl::exec()
{
    for (;;)
    {
        std::cin.clear();
        std::cout << "\u03bb . ";
        std::string input;
        std::getline(std::cin, input);

        // ignore lines without code
        {
            if (input.length() == 0)
            {
                continue;
            }
            bool non_spacing = false;
            for (auto it = input.begin(); it != input.end(); ++it)
            {
                if (*it != ' ' && *it != '\t')
                {
                    non_spacing = true;
                    break;
                }
            }
            if (!non_spacing)
            {
                continue;
            }
        }

        // eval parsed
        try
        {
            auto parser = lisp::Parser(input);
            auto result = lisp::eval(m_env, parser.parse());
            if (result != nullptr && result != lisp::Void::get())
            {
                std::cout << result << std::endl;
            }
        }
        catch (const lisp::Error &e)
        {
            std::cout << e.message() << std::endl;
        }
    }
    return EXIT_SUCCESS;
}
