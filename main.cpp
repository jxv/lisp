#include "lisp.h"
#include "repl.h"

int main(int, const char **)
{
    Repl repl;
    repl.loop();
    return EXIT_SUCCESS;
}
