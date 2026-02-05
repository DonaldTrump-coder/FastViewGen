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
    else if(type == "gamma")
    {
        m_stretch_type = StretchType::GAMMA;
    }
    else if(type == "tile_stretch")
    {
        m_stretch_type = StretchType::TILE_STRETCH;
    }
    else if(type == "piecewise_equal")
    {
        m_stretch_type = StretchType::PIECEWISE_EQUAL;
    }
    else if(type == "histo_equal")
    {
        m_stretch_type = StretchType::HISTO_EQUAL;
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
        Histogram hist(0.0f, 255.0f, 255 * 8);
        #pragma omp parallel for collapse(2)
        for(uint32_t row = 0; row < sat->getHeight(); row++)
        {
            for(uint32_t col = 0; col < sat->getWidth(); col++)
            {
                #pragma omp critical
                hist.add_value(sat->getPixelValue(row, col, 0));
            }
        }
        hist.compute_cdf();
        // get the initial histogram of data

        dark_thres = hist.get_percentile(1.0f);
        bright_thres = hist.get_percentile(99.0f); // The threshold values
        {
            Logger("Dark and bright histogram computed");
        }

        Histogram dark_hist(0.0f, dark_thres, 255 * 4);
        Histogram bright_hist(bright_thres, 255.0f, 255 * 4);
        #pragma omp parallel for collapse(2)
        for(uint32_t row = 0; row < sat->getHeight(); row++)
        {
            for(uint32_t col = 0; col < sat->getWidth(); col++)
            {
                float val = sat->getPixelValue(row, col, 0);
                if(val < dark_thres)
                {
                    #pragma omp critical
                    dark_hist.add_value(val);
                }
                else if(val > bright_thres)
                {
                    #pragma omp critical
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
            float c = (bright_thres - dark_thres) / log(1 + bright_thres - dark_thres);
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
                        sat->setPixelValue(row, col, 0, dark_thres + c * log(1 + val - dark_thres));
                    }
                }
            }
            Logger("PAN data stretched in log mode");
        }
        else if(m_stretch_type == StretchType::GAMMA)
        {
            float bound = bright_thres - dark_thres;
            float gamma = 0.6f;
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
                        float gamma_val = pow((val - dark_thres) / bound, gamma);
                        sat->setPixelValue(row, col, 0, dark_thres + gamma_val * bound);
                    }
                }
            }
            Logger("PAN data stretched in gamma mode");
        }
        else if(m_stretch_type == StretchType::TILE_STRETCH)
        {
            int x_tiles = 5;
            int y_tiles = 5; // num of tiles on different directions
            Histogram* histograms = new Histogram[x_tiles * y_tiles];
            int tile_w = static_cast<int>(sat->getWidth() / x_tiles);
            int tile_h = static_cast<int>(sat->getHeight() / y_tiles);
            int remain_w = sat->getWidth() - tile_w * (x_tiles - 1);
            int remain_h = sat->getHeight() - tile_h * (y_tiles - 1);
            int origin_w = tile_w;
            int origin_h = tile_h;
            for(int i = 0; i < x_tiles; i++) // index tiles
            {
                for(int j = 0; j < y_tiles; j++)
                {
                    if(i == x_tiles - 1)
                    {
                        tile_w = remain_w;
                    }
                    else
                    {
                        tile_w = origin_w;
                    }
                    if(j == y_tiles - 1)
                    {
                        tile_h = remain_h;
                    }
                    else
                    {
                        tile_h = origin_h;
                    }
                    if(i == 0 && j == 0)
                    {
                        // linear stretch in the first tile
                        float min_val = bright_thres;
                        float max_val = dark_thres;
                        #pragma omp parallel for collapse(2) reduction(min:min_val) reduction(max:max_val)
                        for(int x = 0; x < tile_w; x++)
                        {
                            for(int y = 0; y < tile_h; y++)
                            {
                                float val = sat->getPixelValue(y, x, 0);
                                if(val <= bright_thres && val >= dark_thres)
                                {
                                    min_val = std::min(min_val, val);
                                    max_val = std::max(max_val, val);
                                }
                            }
                        } // get min and max
                        
                        #pragma omp parallel for collapse(2)
                        for(int x = 0; x < tile_w; x++)
                        {
                            for(int y = 0; y < tile_h; y++)
                            {
                                float val = sat->getPixelValue(y, x, 0);
                                if(val <= bright_thres && val >= dark_thres)
                                {
                                    float mapped_val = (bright_thres - dark_thres) * (val - min_val) / (max_val - min_val) + dark_thres;
                                    sat->setPixelValue(y, x, 0, mapped_val);
                                }
                            }
                        }
                    }

                    histograms[j * x_tiles + i].set_params(dark_thres, bright_thres, 255 * 6);
                    // calculate in each tile
                    #pragma omp parallel for collapse(2)
                    for(int x = 0; x < tile_w; x++)
                    {
                        if(i * origin_w + x >= (int)sat->getWidth())
                        {
                            continue;
                        }
                        for(int y = 0; y < tile_h; y++)
                        {
                            if(j * origin_h + y >= (int)sat->getHeight())
                            {
                                continue;
                            }
                            float val = sat->getPixelValue(j * origin_h + y, i * origin_w + x, 0);
                            if(val <= bright_thres && val >= dark_thres)
                            {
                                #pragma omp critical
                                histograms[j * x_tiles + i].add_value(val);
                            }
                        }
                    }
                    // calculate histograms in each tile

                    if(i != 0 || j != 0)
                    {
                        // histogram matching with the first tile
                        histograms[j * x_tiles + i].matching(&histograms[0]);

                        // stretch the data in the tile
                        #pragma omp parallel for collapse(2)
                        for(int x = 0; x < tile_w; x++)
                        {
                            if(i * origin_w + x >= (int)sat->getWidth())
                            {
                                continue;
                            }
                            for(int y = 0; y < tile_h; y++)
                            {
                                if(j * origin_h + y >= (int)sat->getHeight())
                                {
                                    continue;
                                }
                                float val = sat->getPixelValue(j * origin_h + y, i * origin_w + x, 0);
                                if(val < dark_thres)
                                {
                                    sat->setPixelValue(j * origin_h + y, i * origin_w + x, 0, dark_hist.mapping(val));
                                }
                                else if(val > bright_thres)
                                {
                                    sat->setPixelValue(j * origin_h + y, i * origin_w + x, 0, bright_hist.mapping(val));
                                }
                                else
                                {
                                    sat->setPixelValue(j * origin_h + y, i * origin_w + x, 0, histograms[j * x_tiles + i].mapping(val));
                                }
                            }
                        }
                    }
                }
            }

            delete[] histograms;
            Logger("PAN data stretched in tile stretch mode");
        }
        else if(m_stretch_type == StretchType::PIECEWISE_EQUAL)
        {
            int num_pieces = 20;
            float interval = (99.0f - 1.0f) / num_pieces;
            float* lower_intervals = new float[num_pieces];
            float* upper_intervals = new float[num_pieces];
            float* lower_thres = new float[num_pieces];
            float* upper_thres = new float[num_pieces];
            Histogram* histograms = new Histogram[num_pieces];
            for(int i = 0; i < num_pieces; i++)
            {
                if(i == 0)
                {
                    lower_intervals[i] = 1.0f;
                    upper_intervals[i] = 1.0f + interval;
                }
                else
                {
                    lower_intervals[i] = upper_intervals[i - 1];
                    upper_intervals[i] = lower_intervals[i] + interval;
                }
                if(i == num_pieces - 1)
                {
                    upper_intervals[i] = 99.0f;
                }
            } // distribute the intervals

            #pragma omp parallel for
            for(int i = 0; i < num_pieces; i++)
            {
                if(i == 0)
                {
                    lower_thres[i] = dark_thres;
                }
                else
                {
                    lower_thres[i] = hist.get_percentile(lower_intervals[i]);
                }
                if(i == num_pieces - 1)
                {
                    upper_thres[i] = bright_thres;
                }
                else
                {
                    upper_thres[i] = hist.get_percentile(upper_intervals[i]);
                }
                histograms[i].set_params(lower_thres[i], upper_thres[i], 255 * 8);
            } // construct interval thresholds and histograms

            // equalization in each interval
            #pragma omp parallel for collapse(2)
            for(uint32_t row = 0; row < sat->getHeight(); row++)
            {
                for(uint32_t col = 0; col < sat->getWidth(); col++)
                {
                    float val = sat->getPixelValue(row, col, 0);
                    if(val >= dark_thres && val <= bright_thres)
                    {
                        for(int i = 0; i < num_pieces; i++)
                        {
                            if(val >= lower_thres[i] && val < upper_thres[i])
                            {
                                #pragma omp critical
                                histograms[i].add_value(val);
                                break;
                            }
                        }
                    }
                }
            }
            #pragma omp parallel for
            for(int i = 0; i < num_pieces; i++)
            {
                histograms[i].equalization();
            }

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
                        for(int i = 0; i < num_pieces; i++)
                        {
                            if(val >= lower_thres[i] && val < upper_thres[i])
                            {
                                sat->setPixelValue(row, col, 0, histograms[i].mapping(val));
                                break;
                            }
                        }
                    }
                }
            }

            delete[] lower_intervals;
            delete[] upper_intervals;
            delete[] lower_thres;
            delete[] upper_thres;
            delete[] histograms;
            Logger("PAN data stretched in piecewise equal mode");
        }
        else if(m_stretch_type == StretchType::HISTO_EQUAL)
        {
            Histogram histogram(dark_thres, bright_thres, 255 * 4);
            #pragma omp parallel for collapse(2)
            for(uint32_t row = 0; row < sat->getHeight(); row++)
            {
                for(uint32_t col = 0; col < sat->getWidth(); col++)
                {
                    float val = sat->getPixelValue(row, col, 0);
                    if(val >= dark_thres && val <= bright_thres)
                    {
                        #pragma omp critical
                        histogram.add_value(val);
                    }
                }
            }

            // equalization
            histogram.equalization();

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
                        sat->setPixelValue(row, col, 0, histogram.mapping(val));
                    }
                }
            }

            Logger("PAN data stretched in histo equal mode");
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

Histogram::Histogram()
{
    return;
}

void Histogram::set_params(float min, float max, int bins)
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
        mapping_histogram[i] = m_min + (float(cdf[i] - cdf_min) / float(cdf_max - cdf_min)) * (m_max - m_min);
    }
}

float Histogram::mapping(float input)
{
    int hist_index = std::min(static_cast<int>((input - m_min) / bin_size), m_bins - 1);
    return mapping_histogram[hist_index];
}

std::vector<int>& Histogram::get_cdf()
{
    return cdf;
}

int Histogram::get_bins()
{
    return m_bins;
}

float Histogram::get_min()
{
    return m_min;
}

float Histogram::get_max()
{
    return m_max;
}

float Histogram::get_bin_size()
{
    return bin_size;
}

void Histogram::matching(Histogram* object_hist)
{
    // histogram matching
    compute_cdf();
    object_hist->compute_cdf();
    int target_idx = 0;
    for(int i = 0; i < m_bins; i++)
    {
        while(target_idx < object_hist->get_bins() - 1 && object_hist->get_cdf()[target_idx] < cdf[i])
        {
            target_idx++;
        }
        mapping_histogram[i] = object_hist->get_min() + (target_idx + 1) * object_hist->get_bin_size();
    }
}