#include <iostream>
#include "tiffio.h"
#include "Logger.h"
#include "Reader.h"

int main() 
{
    {Logger logger(CLEAR_LOG);}
    {Logger logger("App started!");}
    Reader reader("./../data/Satellites/Sat1.TIF");
    return 0;
}