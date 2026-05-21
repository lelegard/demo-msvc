// Implementation of the libfoo library - version with Windows native threads.
// Can be built as a DLL or shared library.
#include <cassert>
#include <windows.h>
#include "libfoo-winthread.h"

// This class uses an internal thread.
// The thread is started in the constructor and terminated in the destructor.
class WinThread
{
public:
    // This class is a singleton.
    static WinThread& instance();

private:
    std::mutex              _mutex {};
    std::condition_variable _cond {};
    HANDLE                  _thread = nullptr;
    bool                    _terminate = false;

    WinThread();
    ~WinThread();
    void thread_main();
    static DWORD WINAPI win_thread_main(LPVOID parameter);
};

// Public API of the library.
void foo_function_winthread()
{
    std::cout << "foo_function_winthread: enter\n" << std::flush;

    // Reference the singleton. It is created the first time it is used.
    WinThread::instance();

    std::cout << "foo_function_winthread: return\n" << std::flush;
}

// Post-C++11 pattern to create a singleton of class WinThread.
WinThread& WinThread::instance()
{
    static WinThread a;
    return a;
}

// Constructor: start the thread.
WinThread::WinThread()
{
    std::cout << "WinThread constructor: enter\n" << std::flush;

    // Assume success for the sake of demo.
    _thread = ::CreateThread(nullptr, 0, win_thread_main, this, 0, nullptr);
    assert(_thread != nullptr);

    std::cout << "WinThread constructor: return\n" << std::flush;
}

// Thread wrapper for Windows native thread.
DWORD WinThread::win_thread_main(LPVOID parameter)
{
    WinThread* obj = reinterpret_cast<WinThread*>(parameter);
    obj->thread_main();
    return 0;
}

// Code of the thread.
void WinThread::thread_main()
{
    std::cout << "WinThread thread: enter\n" << std::flush;
    {
        // Wait for a termination request.
        std::unique_lock<std::mutex> lock(_mutex);
        _cond.wait(lock, [this]() { return _terminate; });
    }
    std::cout << "WinThread thread: return\n" << std::flush;
}

// Destructor: request the thread to terminate and wait for it.
WinThread::~WinThread()
{
    std::cout << "WinThread destructor: enter\n" << std::flush;
    {
        // Send a termination request to the thread.
        std::lock_guard<std::mutex> lock(_mutex);
        _terminate = true;
        _cond.notify_one();
    }

    // Wait for the thread and cleanup.
    ::WaitForSingleObject(_thread, INFINITE);
    ::CloseHandle(_thread);

    std::cout << "WinThread destructor: return\n" << std::flush;
}
