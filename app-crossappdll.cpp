#include "libfoo-crossappdll.h"

// Post-C++11 pattern to create a singleton of class CrossAppDll.
CrossAppDll& CrossAppDll::instance()
{
    static CrossAppDll a;
    return a;
}

// Constructor: start the thread.
CrossAppDll::CrossAppDll()
{
    std::cout << "CrossAppDll constructor: enter\n" << std::flush;
    _thread = new std::thread([this]() { thread_main(); });
    std::cout << "CrossAppDll constructor: return\n" << std::flush;
}

// Code of the thread.
void CrossAppDll::thread_main()
{
    std::cout << "CrossAppDll thread: enter\n" << std::flush;

    // Call a function in the DLL.
    foo_function_crossappdll(*this);

    std::cout << "CrossAppDll thread: return\n" << std::flush;
}

// Destructor: request the thread to terminate and wait for it.
CrossAppDll::~CrossAppDll()
{
    std::cout << "CrossAppDll destructor: enter\n" << std::flush;
    {
        // Send a termination request to the thread.
        std::lock_guard<std::mutex> lock(mutex);
        terminate = true;
        cond.notify_one();
    }

    // Wait for the thread and cleanup.
    _thread->join();
    delete _thread;

    std::cout << "CrossAppDll destructor: return\n" << std::flush;
}
