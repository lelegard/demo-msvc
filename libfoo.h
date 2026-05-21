// Interface of the libfoo library.
#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

// Import/export attributes for DLL.
#if defined(LIBFOODLL_IMPL)
    #define LIBFOODLL __declspec(dllexport)
#elif defined(LIBFOODLL_USE)
    #define LIBFOODLL __declspec(dllimport)
#else
    #define LIBFOODLL
#endif

// Defined in app and libfoo when a DLL is involved.
#if defined(WINDOWS) && (defined(LIBFOODLL_IMPL) || defined(LIBFOODLL_USE))
    #define WITH_LIBFOODLL 1
#endif

// Public API of the library.
LIBFOODLL void foo_function();
