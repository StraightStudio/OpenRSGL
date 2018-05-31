#include <core.h>

int main(int argc, char *argv[])
{
    Core core;
    //
    core.setWindow(1280, 720, "Stratwenty beta");
    core.init();
    //
    return core.exec();
}
