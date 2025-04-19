#include "logger.hpp"
#include "core.hpp"

void Initialize()
{
    shared::Log("Game code initialization!");
}

#ifdef DLL_BUILD
extern "C" __declspec(dllexport) void DLLEndpointInitialize()
{
    shared::Log("DLL endpoint!");
    Initialize();
}
#else
int main(void)
{
    shared::Log("Standalone executable endpoint!");
    Initialize();
}
#endif