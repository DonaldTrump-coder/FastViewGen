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

Reader::~Reader()
{
    if(tif_handle)
    {
        TIFFClose(tif_handle);
        Logger("TIFF file closed");
    }
}