#include <iostream>
#include "tiffio.h"
#include "Logger.h"
#include "Reader.h"
#include "Satellite.h"
#include <yaml-cpp/yaml.h>
#include <string>

int main()
{
    std::string configfile = "D:/Projects/Fastview4Geo/configs/simpletest.yaml";
    {Logger logger(CLEAR_LOG);}
    {Logger logger("App started!");}
    Reader reader("./../data/Satellites/Sat3.tiff");
    std::unique_ptr<Satellite> satellite = reader.get_contents();
    satellite->read_in_buf();
    satellite->normalize();
    YAML::Node config = YAML::LoadFile(configfile);
    {Logger("config loaded");}
    return 0;
}