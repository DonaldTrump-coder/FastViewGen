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
    std::string configfile = "D:/Projects/Fastview4Geo/configs/partitioning.yaml";
    std::string savefolder = "";
    std::string datafile = "";
    std::string stretch_type = "";
    int width = 0;
    int height = 0;
    int n_width = 0;
    int n_height = 0;
    int band1 = 0;
    int band2 = 1;
    int band3 = 2;
    {Logger logger(CLEAR_LOG);}
    {Logger logger("App started!");}

    YAML::Node config = YAML::LoadFile(configfile);
    {Logger("config loaded");}
    if(config["data"])
    {
        datafile = config["data"]["filename"].as<std::string>();
        stretch_type = config["data"]["stretch"].as<std::string>();
    }

    Reader reader(datafile.c_str());
    std::unique_ptr<Satellite> satellite = reader.get_contents();
    satellite->read_in_buf();
    satellite->set_stretch_type(stretch_type);
    satellite->normalize();

    if(config["output"])
    {
        std::string imgtype = config["output"]["imgtype"].as<std::string>();
        if(imgtype == "png" || imgtype == "PNG")
        {
            satellite->set_savetype(ImgSavetype::PNG);
        }
        savefolder = config["output"]["folder"].as<std::string>();
        width = config["output"]["width"].as<int>();
        height = config["output"]["height"].as<int>();
        n_width = config["output"]["n_width"].as<int>();
        n_height = config["output"]["n_height"].as<int>();
        band1 = config["output"]["band1"].as<int>() - 1;
        band2 = config["output"]["band2"].as<int>() - 1;
        band3 = config["output"]["band3"].as<int>() - 1;
    }

    satellite->save_partitioned_img(savefolder, height, width, n_height, n_width, band1, band2, band3);

    return 0;
}