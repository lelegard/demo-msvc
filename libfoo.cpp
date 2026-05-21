// Implementation of the libfoo library.
// Can be built as a DLL or shared library.
#include "libfoo.h"

// This class uses an internal thread.
// The thread is started in the constructor and terminated in the destructor.
class StdThread
{
public:
    // This class is a singleton.
    static StdThread& instance();

private:
    std::mutex              _mutex {};
    std::condition_variable _cond {};
    std::thread*            _thread = nullptr;
    bool                    _terminate = false;

    StdThread();
    ~StdThread();
    void thread_main();
};

// Public API of the library (only one function).
void foo_function()
{
    std::cout << "foo_function: enter\n" << std::flush;

    // Reference the singleton. It is created the first time it is used.
    StdThread::instance();

    std::cout << "foo_function: return\n" << std::flush;
}

// Post-C++11 pattern to create a singleton of class StdThread.
StdThread& StdThread::instance()
{
    static StdThread a;
    return a;
}

// Constructor: start the thread.
StdThread::StdThread()
{
    std::cout << "StdThread constructor: enter\n" << std::flush;
    _thread = new std::thread([this](){ thread_main(); });
    std::cout << "StdThread constructor: return\n" << std::flush;
}

// Code of the thread.
void StdThread::thread_main()
{
    std::cout << "StdThread thread: enter\n" << std::flush;
    {
        // Wait for a termination request.
        std::unique_lock<std::mutex> lock(_mutex);
        _cond.wait(lock, [this]() { return _terminate; });
    }
    std::cout << "StdThread thread: return\n" << std::flush;
}

// Destructor: request the thread to terminate and wait for it.
StdThread::~StdThread()
{
    std::cout << "StdThread destructor: enter\n" << std::flush;
    {
        // Send a termination request to the thread.
        std::lock_guard<std::mutex> lock(_mutex);
        _terminate = true;
        _cond.notify_one();
    }

    // Wait for the thread and cleanup.
    _thread->join();
    delete _thread;

    std::cout << "StdThread destructor: return\n" << std::flush;
}
