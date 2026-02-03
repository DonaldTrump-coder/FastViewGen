#include "Stretch.h"
#include <algorithm>
#include <omp.h>
#include "Logger.h"

float BilinearInterpolation(float f00, float f01, float f10, float f11, float dx, float dy)
{
    float result = f00 * (1 - dx) * (1 - dy) + 
                   f01 * dx * (1 - dy) + 
                   f10 * (1 - dx) * dy + 
                   f11 * dx * dy;
    return result;
}

void Stretch::SetType(SatelliteType type)
{
    m_type = type;
}

void Stretch::set_Stretch_type(std::string& type)
{
    if(type == "linear")
    {
        m_stretch_type = StretchType::LINEAR;
    }
}

void Stretch::normalize_data(Satellite* sat)
{
    if (m_type == PAN)
    {
        f_min = f_max = sat->getPixelValue(0,0,0);
        #pragma omp parallel for reduction(min:f_min) reduction(max:f_max)
        for(uint32_t row = 0; row < sat->getHeight(); row++)
        {
            float row_min = sat->getPixelValue(row, 0, 0);
            float row_max = row_min;
            for(uint32_t col = 0; col < sat->getWidth(); col++)
            {
                float val = sat->getPixelValue(row, col, 0);
                row_min = std::min(row_min, val);
                row_max = std::max(row_max, val);
            }
            f_min = std::min(f_min, row_min);
            f_max = std::max(f_max, row_max);
        }

        float range = f_max - f_min;
        #pragma omp parallel for
        for(uint32_t row = 0; row < sat->getHeight(); row++)
        {
            for(uint32_t col = 0; col < sat->getWidth(); col++)
            {
                float val = sat->getPixelValue(row, col, 0);
                sat->setPixelValue(row, col, 0, (val - f_min) / range * 255.0f);
            }
        }
    }
    else if (m_type == MUL)
    {
        f_min = f_max = sat->getPixelValue(0,0,0);
        for(uint16_t b = 0; b < sat->getBands(); b++)
        {
            #pragma omp parallel for reduction(min:f_min) reduction(max:f_max)
            for(uint32_t row = 0; row < sat->getHeight(); row++)
            {
                float row_min = sat->getPixelValue(row, 0, b);
                float row_max = row_min;
                for(uint32_t col = 0; col < sat->getWidth(); col++)
                {
                    float val = sat->getPixelValue(row, col, b);
                    row_min = std::min(row_min, val);
                    row_max = std::max(row_max, val);
                }
                f_min = std::min(f_min, row_min);
                f_max = std::max(f_max, row_max);
            }
        }

        float range = f_max - f_min;
        #pragma omp parallel for
        for(uint32_t row = 0; row < sat->getHeight(); row++)
        {
            for(uint32_t col = 0; col < sat->getWidth(); col++)
            {
                for(uint16_t b = 0; b < sat->getBands(); b++)
                {
                    float val = sat->getPixelValue(row, col, b);
                    sat->setPixelValue(row, col, b, (val - f_min) / range * 255.0f);
                }
            }
        }
    }
    {
        Logger("Normalized to 0-255");
    }
}

void Stretch::stretch_data(Satellite* sat)
{
    if(m_type == PAN)
    {
        float dark_thres = 0.0f;
        float bright_thres = 255.0f;
        Histogram hist(0.0f, 255.0f, 255 * 2);
        #pragma omp parallel for collapse(2)
        for(uint32_t row = 0; row < sat->getHeight(); row++)
        {
            for(uint32_t col = 0; col < sat->getWidth(); col++)
            {
                hist.add_value(sat->getPixelValue(row, col, 0));
            }
        } // get the initial histogram of data

        if(m_stretch_type == StretchType::LINEAR)
        {
            return;
        }
        if(m_stretch_type == StretchType::LOG)
        {
        }
    }
    else if(m_type == MUL)
    {

    }
    {
        Logger("data stretched");
    }
}

Histogram::Histogram(float min, float max, int bins)
{
    m_min = min;
    m_max = max;
    m_bins = bins;
    bin_size = (m_max - m_min) / m_bins;
    histogram.resize(m_bins, 0);
    mapping_histogram.resize(m_bins, 0);
}

void Histogram::add_value(float value)
{
    int bin_index = static_cast<int>((value - m_min) / bin_size);
    if(bin_index >= 0 && bin_index < m_bins)
    {
        histogram[bin_index]++;
    }
}