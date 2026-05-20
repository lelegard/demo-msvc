# Using Visual Studio for an application with threads in a DLL

This demo illustrates a common singleton pattern of C++ and the danger of
using it in a Windows DLL when this singleton (or static object) contains
an internal thread.

The singleton pattern:

- A class is defined to be instanced only once.
- The constructor is private, which prevents any instance in the application.
- A public static method returns a reference to the unique instance.
- The singleton object is created when it is used for the first time.
- If it is created during the execution of the application, it is automatically
  destructed when the application terminates.

Before C++11, a safe, reliable, and independent of initialization order
implementation of the singleton pattern was a challenge. Starting with C++11,
a simple, reliable pattern is to use a static object inside the instance method.

This pattern works well on all platforms, including Windows, including when
the singleton is inside a DLL.

Things become more complicated when the singleton (or any other static object)
uses internal threads. In that case, the destructor of the static object is in
charge of cleaning up the threads. It requests them to terminate (through some
application-specific protocol) and waits for their termination (`std::thread::join`).

This is legal C++. This is a common pattern. It works well everywhere...
except in a Windows DLL.

This demo application implements a simple application and a library. The library
uses an internal singleton which creates an internal thread. The application+library
pair is built in two ways: with the library as a DLL and as a static library.

With the statically linked application, the internal thread of the singleton runs
after the application returns from `main()` and then it is properly synchronized and
terminated in the static destructor of the singleton.

However, when the library is in a DLL, the internal thread of the singleton has
disappeared when the application returns from `main()`. The static destructor of the
singleton (the code of which is in the DLL) is properly executed but the internal
thread had been killed, somehow, by Windows.

When the same test is run on Linux or macOS, the expected legal C++ behaviour is
observed, whether the library is a static library or a shared library.

In practice, on Windows, the library in the demo application implements two singletons.
One creates a standard C++ `std::thread`. The other one creates a Windows native
thread using `CreateThread()`. The behaviour is identical between C++ thread and
Windows native threads. Therefore, the problem comes from Windows, not from the
C++ runtime library.

When implemented as a static library, the demo application output is:
~~~
main: enter
foo_function: enter
A constructor: enter
A constructor: return
foo_function: return
foo_function_winthread: enter
WinA constructor: enter
WinA constructor: return
foo_function_winthread: return
A thread: enter
WinA thread: enter
main: return               <--- return from main()
WinA destructor: enter     <--- static destructor
WinA thread: return        <--- thread still running
WinA destructor: return
A destructor: enter        <--- static destructor
A thread: return           <--- thread still running
A destructor: return
~~~
This is the expected behaviour.

However, when implemented as a DLL, the demo application output is:
~~~
DllMain: process attach
main: enter
foo_function: enter
A constructor: enter
A constructor: return
DllMain: thread attach
foo_function: return
foo_function_winthread: enter
WinA constructor: enter
WinA constructor: return
foo_function_winthread: return
A thread: enter
DllMain: thread attach
WinA thread: enter
main: return               <--- return from main()
DllMain: process detach
WinA destructor: enter     <--- static destructor, but thread no longer running
WinA destructor: return
A destructor: enter        <--- static destructor, but thread no longer running
A destructor: return
~~~

The "DllMain" traces were added for information only.

There are some new observations here:

- The DLL is "detached" from the process when `main()` returns. Note that
  the DLL is not unloaded. The code of the static destructor is located
  in the DLL and nethertheless executed later.
- The thread starts correctly but it never outputs "thread: return".
  It must have been killed in the meantime, probably when the DLL is
  detached from the process.
- The destructor still executes correctly. Joining the thread which disappeared
  immediately returns. It does not block or crash the application. However,
  if the destructor did expect something from the thread, waiting for some
  explicit response from the thread, it would probably hang.
