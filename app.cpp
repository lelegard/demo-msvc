// Main application.
// Can be linked against the libfoo DLL or static library.
#include "libfoo.h"
#include "libfoo-winthread.h"
#include "libfoo-crossappdll.h"

int main(int argc, char* argv[])
{
    std::cout << "main: enter\n" << std::flush;

    // Call the library (static or dynamic)
    std::cout << "main: calling foo_function\n" << std::flush;
    foo_function();

    // Call the library using Window native threads.
#if defined(WINDOWS)
    std::cout << "main: calling foo_function_winthread\n" << std::flush;
    foo_function_winthread();
#endif

    // Create a thread in app with calls a function in DLL.
#if defined(WITH_LIBFOODLL)
    std::cout << "main: calling CrossAppDll::instance\n" << std::flush;
    CrossAppDll::instance();
#endif

    // Let's pretend we do something
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "main: return\n" << std::flush;
}
