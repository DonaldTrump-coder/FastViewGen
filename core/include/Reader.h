#ifndef READER_H
#define READER_H

#include <tiffio.h>
#include "Satellite.h"
#include <memory>

class Reader 
{
private:
    TIFF* tif_handle = nullptr;
    uint16_t bands = 0; // The num of bands of img
    uint32_t width = 0; // The width of img
    uint32_t height = 0; // The height of img

public:
    Reader(const char* filename);
    ~Reader();
    std::unique_ptr<Satellite> get_contents();
};

#endif // READER_H