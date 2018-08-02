#include <core.h>

int main() // Start here!
{
    setlocale(0, "utf-8"); // utf-8

    Core* core = new Core();

    core->init();

    int status = core->exec();
    delete core;

    return status;
}
