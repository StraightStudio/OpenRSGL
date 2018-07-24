#include <logger.h>

void Logger::log(unistring module, unistring msg)
{
    fprintf(stdout, "\n[L] [ %s ] %s\n", module.c_str(), msg.c_str());
    fflush(stdout);
}

void Logger::err(unistring module, unistring msg)
{
    fprintf(stdout, "\n[E] < [ %s ] - %s >!!!\n", module.c_str(), msg.c_str());
    fflush(stdout);
}

void Logger::warn(unistring module, unistring msg)
{
    fprintf(stdout, "\n[W] { %s } %s\n", module.c_str(), msg.c_str());
    fflush(stdout);
}

void Logger::info(unistring module, unistring msg)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                             module.c_str(),
                             msg.c_str(),
                             nullptr);
}
