#include <include/core.h>

int main()
{
    setlocale(0, "");

    Core core;
    //
    core.init();
    //
    return core.exec();
}
