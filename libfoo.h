// Interface of the libfoo library.

#pragma once

#if defined(_LIBFOODLL_IMPL)
    #define LIBFOODLL __declspec(dllexport)
#elif defined(_LIBFOODLL_USE)
    #define LIBFOODLL __declspec(dllimport)
#else
    #define LIBFOODLL
#endif

// Public API of the library.
LIBFOODLL void foo_function();

// Same with Windows native threads.
#if defined(_WINDOWS)
    LIBFOODLL void foo_function_winthread();
#else
    #define foo_function_winthread() do {} while (false)
#endif
