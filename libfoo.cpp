// Implementation of the libfoo library.
// Can be built as a DLL or shared library.

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "libfoo.h"

// This class uses an internal thread.
// The thread is started in the constructor and terminated in the destructor.
class A
{
public:
    // This class is a singleton.
    static A& instance();

private:
    std::mutex              _mutex {};
    std::condition_variable _cond {};
    std::thread*            _thread = nullptr;
    bool                    _terminate = false;

    A();
    ~A();
    void thread_main();
};

// Public API of the library (only one function).
void foo_function()
{
    std::cout << "foo_function: enter" << std::endl;

    // Reference the singleton. It is created the first time it is used.
    A::instance();

    std::cout << "foo_function: return" << std::endl;
}

// Post-C++11 pattern to create a singleton of class A.
A& A::instance()
{
    static A a;
    return a;
}

// Constructor: start the thread.
A::A()
{
    std::cout << "A constructor: enter" << std::endl;
    _thread = new std::thread([this](){ thread_main(); });
    std::cout << "A constructor: return" << std::endl;
}

// Code of the thread.
void A::thread_main()
{
    std::cout << "thread: enter" << std::endl;
    {
        // Wait for a termination request.
        std::unique_lock<std::mutex> lock(_mutex);
        _cond.wait(lock, [this]() { return _terminate; });
    }
    std::cout << "thread: return" << std::endl;
}

// Destructor: request the thread to terminate and wait for it.
A::~A()
{
    std::cout << "A destructor: enter" << std::endl;
    {
        // Send a termination request to the thread.
        std::lock_guard<std::mutex> lock(_mutex);
        _terminate = true;
        _cond.notify_one();
    }

    // Wait for the thread and cleanup.
    _thread->join();
    delete _thread;

    std::cout << "A destructor: return" << std::endl;
}
