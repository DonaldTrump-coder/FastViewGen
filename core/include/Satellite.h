#ifndef SATELLITE_H
#define SATELLITE_H

#include <tiffio.h>
#include "Logger.h"
#include <vector>
#include "Stretch.h"
#include <algorithm>

enum class ImgSavetype;

class Stretch;

class Satellite 
{
public:
    Satellite(TIFF* tif, uint16_t bands, uint32_t width, uint32_t height);
    virtual void read_in_buf();
    virtual void normalize();
    virtual float getPixelValue(int row, int col, int band);
    virtual void setPixelValue(int row, int col, int band, float value);
    uint32_t getWidth() const;
    uint32_t getHeight() const;
    uint16_t getBands() const;
    //virtual void save_img(const std::string& filename, int left, int top, int right, int bottom, int height, int width);
    virtual void save_whole_img(const std::string& filename, int height, int width, uint16_t band1, uint16_t band2, uint16_t band3);
    void set_savetype(ImgSavetype savetype);
protected:
    TIFF* tif = nullptr;
    uint16_t bands; // The number of bands of img
    uint32_t width; // The width of img
    uint32_t height; // The height of img
    uint16_t bitsPerSample, sampleFormat; // data type of each pixel
    Stretch* stretch; // help to stretch dor the data
    ImgSavetype imgsavetype;
};
// base class of satellite images

class PAN_Satellite : public Satellite
{
public:
    PAN_Satellite(TIFF* tif, uint16_t bands, uint32_t width, uint32_t height);
    void read_in_buf() override;
    ~PAN_Satellite();
    void normalize() override;
    float getPixelValue(int row, int col, int band) override;
    void setPixelValue(int row, int col, int band, float value) override;
    void save_whole_img(const std::string& filename, int height, int width, uint16_t band1, uint16_t band2, uint16_t band3) override;
private:
    uint32_t tile_width = 0; // The width of a tile
    uint32_t tile_length = 0; // The height of a tile
    std::vector<void*> bufVector; // buffer to store the read-in data
};

class MUL_Satellite : public Satellite
{
public:
    MUL_Satellite(TIFF* tif, uint16_t bands, uint32_t width, uint32_t height);
    ~MUL_Satellite();
    void read_in_buf() override;
    void normalize() override;
    float getPixelValue(int row, int col, int band) override;
    void setPixelValue(int row, int col, int band, float value) override;
    void save_whole_img(const std::string& filename, int height, int width, uint16_t band1, uint16_t band2, uint16_t band3) override;
private:
    uint32_t tile_width = 0; // The width of a tile
    uint32_t tile_length = 0; // The height of a tile
    uint16_t planarConfig = 0; // The planar configuration if is not stored in tiles
    std::vector<void*> bufVector; // buffer to store the read-in data
};

#endif // SATELLITE_H