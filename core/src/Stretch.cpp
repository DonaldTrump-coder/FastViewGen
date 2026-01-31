#include "Stretch.h"
#include <algorithm>
#include <omp.h>
#include "Logger.h"

void Stretch::SetType(SatelliteType type)
{
    m_type = type;
}

void Stretch::normalize_data(Satellite* sat)
{
    if (m_type == PAN)
    {
        f_min = f_max = sat->getPixelValue(0,0,0);
        #pragma omp parallel for reduction(min:f_min) reduction(max:f_max)
        for(uint32_t row = 0; row < sat->getWidth(); row++)
        {
            float row_min = sat->getPixelValue(row, 0, 0);
            float row_max = row_min;
            for(uint32_t col = 0; col < sat->getHeight(); col++)
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
        for(uint32_t row = 0; row < sat->getWidth(); row++)
        {
            for(uint32_t col = 0; col < sat->getHeight(); col++)
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
            for(uint32_t row = 0; row < sat->getWidth(); row++)
            {
                float row_min = sat->getPixelValue(row, 0, b);
                float row_max = row_min;
                for(uint32_t col = 0; col < sat->getHeight(); col++)
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
        for(uint32_t row = 0; row < sat->getWidth(); row++)
        {
            for(uint32_t col = 0; col < sat->getHeight(); col++)
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