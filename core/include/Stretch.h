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
    PIECEWISE_EQUAL,
    HISTO_EQUAL
};

class Histogram
{
public:
    Histogram();
    Histogram(float min, float max, int bins);
    void set_params(float min, float max, int bins);
    void add_value(float value);
    void compute_cdf();
    float get_percentile(float perc);
    void equalization();
    float mapping(float input);
    std::vector<int>& get_cdf();
    int get_bins();
    float get_min();
    float get_max();
    float get_bin_size();
    void matching(Histogram* object_hist);
private:
    float m_min, m_max; // min and max values of the histogram
    float bin_size; // size of each bin
    int m_bins; // number of bins
    std::vector<int> histogram; // histogram data
    std::vector<int> cdf; // cumulative distribution function data
    std::vector<float> mapping_histogram; // mapping histogram data
};

float BilinearInterpolation(float f00, float f01, float f10, float f11, float dx, float dy);

class Stretch
{
private:
    SatelliteType m_type;
    float f_min, f_max;
    StretchType m_stretch_type = StretchType::LINEAR;
public:
    void SetType(SatelliteType type);
    void normalize_data(Satellite* sat);
    void set_Stretch_type(std::string& type);
    void stretch_data(Satellite* sat);
    void stretch_data(Satellite* sat, uint16_t band1, uint16_t band2, uint16_t band3);
    StretchType get_Stretch_type();
};

struct HSI
{
    float h, s, i;
};

struct RGB
{
    float r, g, b;
};

HSI rgb2hsi(float r, float g, float b);
RGB hsi2rgb(float h, float s, float i);

#endif // STRETCH_H