#ifndef LOGGER_H
#define LOGGER_H

#ifdef _WIN64
#include <depends.h>
#else
#include <include/depends.h>
#endif

class Logger {
public:
    static void log(unistring module, unistring msg);

    static void err(unistring module, unistring msg);

    static void warn(unistring module, unistring msg);

    static void info(unistring module, unistring msg);
private:
    Logger(){}
};

#endif // LOGGER_H
