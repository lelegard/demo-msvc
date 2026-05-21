#include "libfoo.h"

#if defined(WINDOWS)

// Same test as libfoo with Windows native threads instead of C++ threads.
LIBFOODLL void foo_function_winthread();

#endif
