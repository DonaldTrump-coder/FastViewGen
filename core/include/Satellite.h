#ifndef SATELLITE_H
#define SATELLITE_H

#include <tiffio.h>
#include "Logger.h"

class Satellite 
{
public:
    Satellite(TIFF* tif, uint16_t bands, uint32_t width, uint32_t height);
private:
    TIFF* tif = nullptr;
    uint16_t bands; // The number of bands of img
    uint32_t width; // The width of img
    uint32_t height; // The height of img
};
// base class of satellite images

class PAN_Satellite : public Satellite
{
public:
    PAN_Satellite(TIFF* tif, uint16_t bands, uint32_t width, uint32_t height);
private:

};

class MUL_Satellite : public Satellite
{
public:
    MUL_Satellite(TIFF* tif, uint16_t bands, uint32_t width, uint32_t height);
private:
};

#endif // SATELLITE_H