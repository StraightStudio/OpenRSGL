#ifndef LOGGER_H
#define LOGGER_H

#include <depends.h>

class Logger
{
public:
    Logger();
    void log(QString module, QString msg);
    void err(QString module, QString msg);
    void warn(QString module, QString msg);
};

#endif // LOGGER_H
