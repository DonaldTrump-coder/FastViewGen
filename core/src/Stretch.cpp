#include "Stretch.h"
#include <algorithm>
#include <omp.h>
#include "Logger.h"
#include <cmath>

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
    else if(type == "log")
    {
        m_stretch_type = StretchType::LOG;
    }
}

void Stretch::normalize_data(Satellite* sat)
{
    if (m_type == PAN)
    {
        f_min = f_max = sat->getPixelValue(0,0,0);
        #pragma omp parallel
        {
            float local_min = f_min;
            float local_max = f_max;

            #pragma omp for
            for(uint32_t row = 0; row < sat->getHeight(); row++)
            {
                float row_min = sat->getPixelValue(row, 0, 0);
                float row_max = row_min;

                #pragma omp parallel for reduction(min:row_min) reduction(max:row_max)
                for(uint32_t col = 0; col < sat->getWidth(); col++)
                {
                    float val = sat->getPixelValue(row, col, 0);
                    row_min = std::min(row_min, val);
                    row_max = std::max(row_max, val);
                }

                #pragma omp critical
                {
                    f_min = std::min(f_min, row_min);
                    f_max = std::max(f_max, row_max);
                }
            }
        }

        float range = f_max - f_min;
        #pragma omp parallel for collapse(2)
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
        #pragma omp parallel for collapse(2)
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
        Histogram hist(0.0f, 255.0f, 255 * 6);
        #pragma omp parallel for collapse(2)
        for(uint32_t row = 0; row < sat->getHeight(); row++)
        {
            for(uint32_t col = 0; col < sat->getWidth(); col++)
            {
                hist.add_value(sat->getPixelValue(row, col, 0));
            }
        }
        hist.compute_cdf();
        // get the initial histogram of data

        dark_thres = hist.get_percentile(0.1f);
        bright_thres = hist.get_percentile(99.99f); // The threshold values
        {
            Logger("Dark and bright histogram computed");
        }

        Histogram dark_hist(0.0f, dark_thres, 255 * 3);
        Histogram bright_hist(bright_thres, 255.0f, 255 * 3);
        #pragma omp parallel for collapse(2)
        for(uint32_t row = 0; row < sat->getHeight(); row++)
        {
            for(uint32_t col = 0; col < sat->getWidth(); col++)
            {
                float val = sat->getPixelValue(row, col, 0);
                if(val < dark_thres)
                {
                    dark_hist.add_value(val);
                }
                else if(val > bright_thres)
                {
                    bright_hist.add_value(val);
                }
            }
        }
        dark_hist.equalization();
        bright_hist.equalization(); // Get the mapping of dark and bright areas

        if(m_stretch_type == StretchType::LINEAR)
        {
            #pragma omp parallel for collapse(2)
            for(uint32_t row = 0; row < sat->getHeight(); row++)
            {
                for(uint32_t col = 0; col < sat->getWidth(); col++)
                {
                    float val = sat->getPixelValue(row, col, 0);
                    if(val < dark_thres)
                    {
                        sat->setPixelValue(row, col, 0, dark_hist.mapping(val));
                    }
                    else if(val > bright_thres)
                    {
                        sat->setPixelValue(row, col, 0, bright_hist.mapping(val));
                    }
                }
            }
            Logger("PAN data stretched in linear mode");
        }
        else if(m_stretch_type == StretchType::LOG)
        {
            #pragma omp parallel for collapse(2)
            for(uint32_t row = 0; row < sat->getHeight(); row++)
            {
                for(uint32_t col = 0; col < sat->getWidth(); col++)
                {
                    float val = sat->getPixelValue(row, col, 0);
                    if(val < dark_thres)
                    {
                        sat->setPixelValue(row, col, 0, dark_hist.mapping(val));
                    }
                    else if(val > bright_thres)
                    {
                        sat->setPixelValue(row, col, 0, bright_hist.mapping(val));
                    }
                    else
                    {
                        // stretch the data in logarithmic scale
                        float c = (bright_thres - dark_thres) / log(1 + bright_thres - dark_thres);
                        sat->setPixelValue(row, col, 0, dark_thres + c * log(1 + val - dark_thres));
                    }
                }
            }
            Logger("PAN data stretched in log mode");
        }
        else if(m_stretch_type == StretchType::GAMMA)
        {
            
        }
    }
}

StretchType Stretch::get_Stretch_type()
{
    return m_stretch_type;
}

Histogram::Histogram(float min, float max, int bins)
{
    m_min = min;
    m_max = max;
    m_bins = bins;
    bin_size = (m_max - m_min) / m_bins;
    histogram.resize(m_bins, 0);
    cdf.resize(m_bins, 0);
    mapping_histogram.resize(m_bins, 0.0f);
}

void Histogram::add_value(float value)
{
    int bin_index = std::min(static_cast<int>((value - m_min) / bin_size), m_bins - 1);
    if(bin_index >= 0 && bin_index < m_bins)
    {
        histogram[bin_index]++;
    }
}

void Histogram::compute_cdf()
{
    cdf[0] = histogram[0];
    for(int i = 1; i < m_bins; i++)
    {
        cdf[i] = cdf[i - 1] + histogram[i];
    }
}

float Histogram::get_percentile(float perc)
{
    int total = cdf.back();
    int target_count = (int)(perc * total / 100);
    for(int i = 0; i < m_bins; i++)
    {
        if(cdf[i] >= target_count)
        {
            return m_min + (i + 1) * bin_size;
        }
    }
    return m_max;
}

void Histogram::equalization()
{
    compute_cdf();
    int cdf_min = cdf[0];
    int cdf_max = cdf.back();
    #pragma omp parallel for
    for(int i = 0; i < m_bins; i++)
    {
        mapping_histogram[i] = m_min + ((cdf[i] - cdf_min) / (cdf_max - cdf_min)) * (m_max - m_min);
    }
}

float Histogram::mapping(float input)
{
    int hist_index = std::min(static_cast<int>((input - m_min) / bin_size), m_bins - 1);
    return mapping_histogram[hist_index];
}