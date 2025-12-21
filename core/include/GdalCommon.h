//Jiahao Zhou and Haojun Tang
//2025-2026
//git@github.com:Jeiluo/Fastview4Geo.git

#ifndef GDALCOMMON_H
#define GDALCOMMON_H

#include "gdal_priv.h"
#include "cpl_conv.h"
#include "ogr_spatialref.h"

// GDAL 初始化
class GdalInitializer {
public:
    static void init();
};

#endif // GDALCOMMON_H