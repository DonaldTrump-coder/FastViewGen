//Jiahao Zhou, Haojun Tang
//2025-2026
//git@github.com:Jeiluo/Fastview4Geo.git

//GDAL 初始化类 GdalInitializer,提供静态初始化方法

#ifndef GDALCOMMON_H
#define GDALCOMMON_H

#include "gdal_priv.h"
#include "cpl_conv.h"
#include "mutex"

// GDAL 初始化
class GdalInitializer {
public:
    static void init(){
        static std::once_flag flag;
        std::call_once(flag, []() {
            // 注册所有 GDAL 驱动
            GDALAllRegister();

#ifdef _WIN32
            // 支持中文路径
            CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "YES");
            CPLSetConfigOption("SHAPE_ENCODING", "");
#endif
        });
    }
};

#endif // GDALCOMMON_H