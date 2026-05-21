#include "libfoo-crossappdll.h"

// Called from a thread CrossAppDll which was created in app.
void foo_function_crossappdll(CrossAppDll& cad)
{
    std::cout << "foo_function_crossappdll: enter\n" << std::flush;
    {
        // Wait for a termination request.
        std::unique_lock<std::mutex> lock(cad.mutex);
        cad.cond.wait(lock, [&cad]() { return cad.terminate; });
    }
    std::cout << "foo_function_crossappdll: return\n" << std::flush;
}
