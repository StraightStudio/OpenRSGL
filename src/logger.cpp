#include <include/logger.h>

void Logger::log(unistring module, unistring msg)
{
    fprintf(stdout, "\n[L] [ %s ] %s\n", module.c_str(), msg.c_str());
    fflush(stdout);
}

void Logger::err(unistring module, unistring msg)
{
    fprintf(stdout, "\n[E] < [ %s ] - %s >!!!\n", module.c_str(), msg.c_str());
    fflush(stdout);
    exit(-1);
}

void Logger::warn(unistring module, unistring msg)
{
    fprintf(stdout, "\n[W] { %s } %s\n", module.c_str(), msg.c_str());
    fflush(stdout);
}
