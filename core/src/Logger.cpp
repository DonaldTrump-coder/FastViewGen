#include "Logger.h"

Logger::Logger(const std::string& log)
{
    outFile.open(logfilename, std::ios::out | std::ios::app);
    outFile << log << std::endl;
}

Logger::Logger(LogMode mode)
{
    if (mode == CLEAR_LOG)
    {
        outFile.open(logfilename,  std::ios::out | std::ios::trunc);
    }
}

Logger::~Logger()
{
    outFile.close();
}