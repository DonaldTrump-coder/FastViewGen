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