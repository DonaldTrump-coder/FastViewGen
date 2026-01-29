#include "Satellite.h"

Satellite::Satellite(TIFF* tif, uint16_t bands, uint32_t width, uint32_t height)
{
    this->tif = tif;
    this->bands = bands;
    this->width = width;
    this->height = height;
}

PAN_Satellite::PAN_Satellite(TIFF* tif, uint16_t bands, uint32_t width, uint32_t height) : Satellite(tif, bands, width, height)
{
    
}

MUL_Satellite::MUL_Satellite(TIFF* tif, uint16_t bands, uint32_t width, uint32_t height) : Satellite(tif, bands, width, height)
{

}