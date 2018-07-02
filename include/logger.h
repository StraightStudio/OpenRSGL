#ifndef LOGGER_H
#define LOGGER_H

#include <include/depends.h>

class Logger {
public:
    static void log(unistring module, unistring msg);

    static void err(unistring module, unistring msg);

    static void warn(unistring module, unistring msg);
private:
    Logger(){}
};

#endif // LOGGER_H
