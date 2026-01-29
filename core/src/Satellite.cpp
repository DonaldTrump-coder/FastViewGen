#include "Satellite.h"
#include <thread>

Satellite::Satellite(TIFF* tif, uint16_t bands, uint32_t width, uint32_t height)
{
    this->tif = tif;
    this->bands = bands;
    this->width = width;
    this->height = height;
}

void Satellite::read_in_buf()
{
    return;
}

PAN_Satellite::PAN_Satellite(TIFF* tif, uint16_t bands, uint32_t width, uint32_t height) : Satellite(tif, bands, width, height)
{
    if(TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width))
    {
        Logger("Tile width: " + std::to_string(tile_width));
    }
    if(TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_length))
    {
        Logger("Tile length: " + std::to_string(tile_length));
    } // get the tiles size
    if(TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitsPerSample))
    {
        Logger("Bits per sample: " + std::to_string(bitsPerSample));
    }
    if(TIFFGetField(tif, TIFFTAG_SAMPLEFORMAT, &sampleFormat))
    {
        Logger("Sample format: " + std::to_string(sampleFormat));
    }
}

void PAN_Satellite::read_in_buf()
{
    if(tile_length == 0 || tile_width == 0)
    {
        // read in lines
        if (bitsPerSample == 8 && sampleFormat == 1)
        {
            // uint8 data
            for (uint32_t i = 0; i < height; i++)
            {
                uint8_t* buf = new uint8_t[width];
                bufVector.push_back(buf);
            }
            Logger("type is uint8");
        }
        else if (bitsPerSample == 16 && sampleFormat == 1)
        {
            // uint16 data
            for (uint32_t i = 0; i < height; i++)
            {
                uint16_t* buf = new uint16_t[width];
                bufVector.push_back(buf);
            }
            Logger("type is uint16");
        }
        else if (bitsPerSample == 32 && sampleFormat == 3)
        {
            // float data
            for (uint32_t i = 0; i < height; i++)
            {
                float* buf = new float[width];
                bufVector.push_back(buf);
            }
            Logger("type is float32");
        }
        else
        {
            Logger("Unsupported data type in PAN_Satellite::read_in_buf()");
            return;
        }
    }
}

MUL_Satellite::MUL_Satellite(TIFF* tif, uint16_t bands, uint32_t width, uint32_t height) : Satellite(tif, bands, width, height)
{
    if(TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width))
    {
        Logger("Tile width: " + std::to_string(tile_width));
    }
    if(TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_length))
    {
        Logger("Tile length: " + std::to_string(tile_length));
    } // get the tiles size
    if (TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planarConfig))
    {
        Logger("Planar Configuration: " + std::to_string(planarConfig));
    }
    if(TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitsPerSample))
    {
        Logger("Bits per sample: " + std::to_string(bitsPerSample));
    }
    if(TIFFGetField(tif, TIFFTAG_SAMPLEFORMAT, &sampleFormat))
    {
        Logger("Sample format: " + std::to_string(sampleFormat));
    }
}

void MUL_Satellite::read_in_buf()
{
    
}