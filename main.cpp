#include <include/core.h>

int main()
{
    Core core;
    //
    core.init();
    core.setWindow(1280, 720, "Stratwenty beta");
    //
    return core.exec();
}
