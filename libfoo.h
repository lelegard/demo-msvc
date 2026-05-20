// Interface of the libfoo library.

#pragma once

#if defined(_LIBFOODLL_IMPL)
    #define LIBFOODLL __declspec(dllexport)
#elif defined(_LIBFOODLL_USE)
    #define LIBFOODLL __declspec(dllimport)
#else
    #define LIBFOODLL
#endif

// Public API of the library (only one function).
LIBFOODLL void foo_function();
