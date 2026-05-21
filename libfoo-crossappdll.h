#include "libfoo.h"

#if defined(WITH_LIBFOODLL)

// This class uses an internal thread.
// The thread is started in the constructor and terminated in the destructor.
// The object and the thread are created in the main application but the
// thread executes a function in the DLL when main() terminates.
class CrossAppDll
{
public:
    // This class is a singleton.
    static CrossAppDll& instance();

    // These fields are public so they can be used in the function in the DLL.
    std::mutex              mutex{};
    std::condition_variable cond{};
    bool                    terminate = false;

private:
    std::thread* _thread = nullptr;

    CrossAppDll();
    ~CrossAppDll();
    void thread_main();
};

// This function is executed in the DLL by the thread.
LIBFOODLL void foo_function_crossappdll(CrossAppDll& cad);

#endif
