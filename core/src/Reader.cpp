#include "Reader.h"
#include "Logger.h"

Reader::Reader(const char* filename) 
{
    tif_handle = TIFFOpen(filename, "r");
    if (!tif_handle)
    {
        Logger("Failed to open TIFF file.");
    }

    if(TIFFGetField(tif_handle, TIFFTAG_SAMPLESPERPIXEL, &bands))
    {
        Logger("Number of bands: " + std::to_string(bands));
    }
    if(TIFFGetField(tif_handle, TIFFTAG_IMAGEWIDTH, &width))
    {
        Logger("Image width: " + std::to_string(width));
    }
    if(TIFFGetField(tif_handle, TIFFTAG_IMAGELENGTH, &height))
    {
        Logger("Image height: " + std::to_string(height));
    }
}

std::unique_ptr<Satellite> Reader::get_contents()
{
    if(bands == 0)
    {
        return std::unique_ptr<Satellite>(new Satellite(nullptr, 0, 0, 0));
    }
    else if(bands == 1)
    {
        return std::unique_ptr<PAN_Satellite>(new PAN_Satellite(tif_handle, bands, width, height));
    }
    else
    {
        return std::unique_ptr<MUL_Satellite>(new MUL_Satellite(tif_handle, bands, width, height));
    }
}

Reader::~Reader()
{
    if(tif_handle)
    {
        TIFFClose(tif_handle);
        Logger("TIFF file closed");
    }
}