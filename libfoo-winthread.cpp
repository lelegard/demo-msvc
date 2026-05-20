// Implementation of the libfoo library - version with Windows native threads.
// Can be built as a DLL or shared library.

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cassert>
#include <windows.h>
#include "libfoo.h"

// This class uses an internal thread.
// The thread is started in the constructor and terminated in the destructor.
class WinA
{
public:
    // This class is a singleton.
    static WinA& instance();

private:
    std::mutex              _mutex {};
    std::condition_variable _cond {};
    HANDLE                  _thread = nullptr;
    bool                    _terminate = false;

    WinA();
    ~WinA();
    void thread_main();
    static DWORD WINAPI win_thread_main(LPVOID parameter);
};

// Public API of the library.
void foo_function_winthread()
{
    std::cout << "foo_function_winthread: enter" << std::endl;

    // Reference the singleton. It is created the first time it is used.
    WinA::instance();

    std::cout << "foo_function_winthread: return" << std::endl;
}

// Post-C++11 pattern to create a singleton of class A.
WinA& WinA::instance()
{
    static WinA a;
    return a;
}

// Constructor: start the thread.
WinA::WinA()
{
    std::cout << "WinA constructor: enter" << std::endl;

    // Assume success for the sake of demo.
    _thread = ::CreateThread(nullptr, 0, win_thread_main, this, 0, nullptr);
    assert(_thread != nullptr);

    std::cout << "WinA constructor: return" << std::endl;
}

// Thread wrapper for Windows native thread.
DWORD WinA::win_thread_main(LPVOID parameter)
{
    WinA* obj = reinterpret_cast<WinA*>(parameter);
    obj->thread_main();
    return 0;
}

// Code of the thread.
void WinA::thread_main()
{
    std::cout << "WinA thread: enter" << std::endl;
    {
        // Wait for a termination request.
        std::unique_lock<std::mutex> lock(_mutex);
        _cond.wait(lock, [this]() { return _terminate; });
    }
    std::cout << "WinA thread: return" << std::endl;
}

// Destructor: request the thread to terminate and wait for it.
WinA::~WinA()
{
    std::cout << "WinA destructor: enter" << std::endl;
    {
        // Send a termination request to the thread.
        std::lock_guard<std::mutex> lock(_mutex);
        _terminate = true;
        _cond.notify_one();
    }

    // Wait for the thread and cleanup.
    ::WaitForSingleObject(_thread, INFINITE);
    ::CloseHandle(_thread);

    std::cout << "WinA destructor: return" << std::endl;
}
