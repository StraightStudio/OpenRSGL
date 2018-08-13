#include <core.h>

int main(int argc, char **argv) // Start here!
{
    setlocale(0, "utf-8"); // utf-8

    Core* core = new Core();

    if(argc > 1)
    {
        fprintf(stdout, "More than 1 parameter!");
        core->init(true);
    }
    else
    {
        core->init(false);
        core->connect("127.0.0.1", GAME_PORT);
    }

    int status = core->exec();
    delete core;

    return status;
}
