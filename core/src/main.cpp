#include "../include/GdalCommon.h"
#include <iostream>

int main() {
    GdalInitializer::init();
    std::cout << "GDAL Initialized successfully." << std::endl;
    return 0;
}