#ifndef LOGGER_H
#define LOGGER_H

#include <include/depends.h>

class Logger {
public:
    static void log(QString module, QString msg);

    static void err(QString module, QString msg);

    static void warn(QString module, QString msg);
private:
    Logger(){}
};

#endif // LOGGER_H
