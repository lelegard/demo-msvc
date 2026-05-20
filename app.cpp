// Main application.
// Can be linked against the libfoo DLL or static library.

#include <iostream>
#include <chrono>
#include <thread>
#include "libfoo.h"

int main(int argc, char* argv[])
{
    std::cout << "main: enter" << std::endl;

    // Call the library (static or dynamic)
    foo_function();

    // Let's pretend we do something
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "main: return" << std::endl;
}
