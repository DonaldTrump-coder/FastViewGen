#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <iostream>

enum LogMode
{
    CLEAR_LOG
};

class Logger 
{
private:
    std::string logfilename = "log.txt";
    std::ofstream outFile;
public:
    Logger(const std::string& log);
    Logger(LogMode mode);
    ~Logger();
};

#endif // LOGGER_H