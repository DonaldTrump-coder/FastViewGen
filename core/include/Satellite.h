#ifndef SATELLITE_H
#define SATELLITE_H

#include <tiffio.h>
#include "Logger.h"
#include <vector>

class Satellite 
{
public:
    Satellite(TIFF* tif, uint16_t bands, uint32_t width, uint32_t height);
    virtual void read_in_buf();
protected:
    TIFF* tif = nullptr;
    uint16_t bands; // The number of bands of img
    uint32_t width; // The width of img
    uint32_t height; // The height of img
    uint16_t bitsPerSample, sampleFormat; // data type of each pixel
};
// base class of satellite images

class PAN_Satellite : public Satellite
{
public:
    PAN_Satellite(TIFF* tif, uint16_t bands, uint32_t width, uint32_t height);
    void read_in_buf() override;
    ~PAN_Satellite();
private:
    uint32_t tile_width = 0; // The width of a tile
    uint32_t tile_length = 0; // The height of a tile
    std::vector<void*> bufVector; // buffer to store the read-in data

    template <typename T>
    T getPixelValue(int row, int col);
};

class MUL_Satellite : public Satellite
{
public:
    MUL_Satellite(TIFF* tif, uint16_t bands, uint32_t width, uint32_t height);
    ~MUL_Satellite();
    void read_in_buf() override;
private:
    uint32_t tile_width = 0; // The width of a tile
    uint32_t tile_length = 0; // The height of a tile
    uint16_t planarConfig = 0; // The planar configuration if is not stored in tiles
    std::vector<void*> bufVector; // buffer to store the read-in data

    template <typename T>
    T getPixelValue(int band, int row, int col);
};

#endif // SATELLITE_H