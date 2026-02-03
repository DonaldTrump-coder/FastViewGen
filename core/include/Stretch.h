#ifndef STRETCH_H
#define STRETCH_H

#include <tiffio.h>
#include <vector>
#include "Satellite.h"
#include <string>

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

enum class StretchType
{
    LINEAR,
    LOG,
    GAMMA,
    TILE_STRETCH,
    PIECEWISE_LINEAR,
    HISTO_EQUAL
};

class Histogram
{
public:
    Histogram(float min, float max, int bins);
    void add_value(float value);
private:
    float m_min, m_max; // min and max values of the histogram
    float bin_size; // size of each bin
    int m_bins; // number of bins
    std::vector<int> histogram; // histogram data
    std::vector<int> mapping_histogram; // mapping histogram data
};

float BilinearInterpolation(float f00, float f01, float f10, float f11, float dx, float dy);

class Stretch
{
private:
    SatelliteType m_type;
    float f_min, f_max;
    StretchType m_stretch_type;
public:
    void SetType(SatelliteType type);
    void normalize_data(Satellite* sat);
    void set_Stretch_type(std::string& type);
    void stretch_data(Satellite* sat);
};

#endif // STRETCH_H