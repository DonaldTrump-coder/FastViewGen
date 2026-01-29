#include <iostream>
#include "tiffio.h"
#include "Logger.h"
#include "Reader.h"
#include "Satellite.h"

int main() 
{
    {Logger logger(CLEAR_LOG);}
    {Logger logger("App started!");}
    Reader reader("./../data/Satellites/Sat2.TIF");
    std::unique_ptr<Satellite> satellite = reader.get_contents();
    satellite->read_in_buf();
    return 0;
}