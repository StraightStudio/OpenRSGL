
#ifdef _WIN64
#include <core.h>
#else 
#include <include/core.h>
#endif

int main()
{
    setlocale(0, "");

    Core core;
    //
    core.init();
    //
    return core.exec();
}
