#ifndef STRETCH_H
#define STRETCH_H

#include <tiffio.h>
#include <vector>
#include "Satellite.h"

class Satellite;

enum SatelliteType 
{
    PAN,
    MUL
};

enum class ImgSavetype
{
    PNG
};

float BilinearInterpolation(float f00, float f01, float f10, float f11, float dx, float dy);

class Stretch
{
private:
    SatelliteType m_type;
    float f_min, f_max;
public:
    void SetType(SatelliteType type);
    void normalize_data(Satellite* sat);
};

#endif // STRETCH_H