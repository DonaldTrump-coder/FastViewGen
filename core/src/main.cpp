#include <iostream>
#include "tiffio.h"
#include "Logger.h"
#include "Reader.h"
#include "Satellite.h"
#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>
#include <string>

int main()
{
    std::string configfile = "D:/Projects/Fastview4Geo/configs/simpletest.yaml";
    std::string savefile = "";
    int width = 0;
    int height = 0;
    {Logger logger(CLEAR_LOG);}
    {Logger logger("App started!");}
    Reader reader("./../data/Satellites/Sat3.tiff");
    std::unique_ptr<Satellite> satellite = reader.get_contents();
    satellite->read_in_buf();
    satellite->normalize();

    YAML::Node config = YAML::LoadFile(configfile);
    {Logger("config loaded");}

    if(config["output"])
    {
        std::string imgtype = config["output"]["imgtype"].as<std::string>();
        if(imgtype == "png" || imgtype == "PNG")
        {
            satellite->set_savetype(ImgSavetype::PNG);
        }
        savefile = config["output"]["filename"].as<std::string>();
        width = config["output"]["width"].as<int>();
        height = config["output"]["height"].as<int>();
    }

    satellite->save_whole_img(savefile, height, width, 0, 1, 2);

    return 0;
}