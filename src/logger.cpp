#include "../include/logger.h"

void Logger::log(QString module, QString msg)
{
    fprintf(stdout, "\n[L] [ %s ] %s\n", module.toStdString().c_str(), msg.toStdString().c_str());
    fflush(stdout);
}

void Logger::err(QString module, QString msg)
{
    fprintf(stdout, "\n[E] < [ %s ] - %s >!!!\n", module.toStdString().c_str(), msg.toStdString().c_str());
    fflush(stdout);
}

void Logger::warn(QString module, QString msg)
{
    fprintf(stdout, "\n[W] { %s } %s\n", module.toStdString().c_str(), msg.toStdString().c_str());
    fflush(stdout);
}
