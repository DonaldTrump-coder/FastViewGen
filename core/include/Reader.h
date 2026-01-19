#include <tiffio.h>

class Reader 
{
private:
    TIFF* tif_handle = nullptr;
    uint16_t bands;
    uint32_t width;
    uint32_t height;

public:
    Reader(const char* filename);
    ~Reader();
};