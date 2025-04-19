#include "stdio.h"

namespace shared
{
    void Log(const char* str)
    {
        printf("%s\n", str);
    }
}