// Additional module in libfoo DLL.
// Just to trace DLL loading and unloading information.

#include <iostream>
#include <windows.h>

bool APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
    switch (reason) {
        case DLL_PROCESS_ATTACH:
            std::cout << "DllMain: process attach\n" << std::flush;
            break;
        case DLL_THREAD_ATTACH:
            std::cout << "DllMain: thread attach\n" << std::flush;
            break;
        case DLL_THREAD_DETACH:
            std::cout << "DllMain: thread detach\n" << std::flush;
            break;
        case DLL_PROCESS_DETACH:
            std::cout << "DllMain: process detach\n" << std::flush;
            break;
    }
    return true;
}
