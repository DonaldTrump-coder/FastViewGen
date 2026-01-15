#include <iostream>
#include "tiffio.h"

int main() 
{
    TIFF* tif = TIFFOpen("../data/TIFFExamples/BigTIFF.tif", "r");
    TIFFClose(tif);
    std::cout<< "TIFF file read successfully."<< std::endl;
    return 0;
}