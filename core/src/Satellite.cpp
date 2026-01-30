#include "Satellite.h"

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

void Satellite::normalize()
{
    return;
}

float Satellite::getPixelValue(int row, int col, int band)
{
    return 0.0f;
}

void Satellite::setPixelValue(int row, int col, int band, float value)
{
    return;
}

uint32_t Satellite::getWidth() const
{
    return width;
}

uint32_t Satellite::getHeight() const
{
    return height;
}

uint16_t Satellite::getBands() const
{
    return bands;
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
            for (uint32_t row = 0; row < height; row++)
            {
                uint8_t* buf = new uint8_t[width];
                bufVector.push_back(buf);
            }
            Logger("type is uint8");
        }
        else if (bitsPerSample == 16 && sampleFormat == 1)
        {
            // uint16 data
            for (uint32_t row = 0; row < height; row++)
            {
                uint16_t* buf = new uint16_t[width];
                bufVector.push_back(buf);
            }
            Logger("type is uint16");
        }
        else if (bitsPerSample == 32 && sampleFormat == 3)
        {
            // float data
            for (uint32_t row = 0; row < height; row++)
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
        
        #pragma omp parallel for
        for(uint32_t row = 0; row < height; row++)
        {
            TIFFReadScanline(tif, bufVector[row], row);
        }
    }
    {
        Logger("read in data done");
    }
    // The data is stored in bufVector
}

float PAN_Satellite::getPixelValue(int row, int col, int band)
{
    float* rowData = static_cast<float*>(bufVector[row]);
    return rowData[col];
}

void PAN_Satellite::setPixelValue(int row, int col, int band, float value)
{
    static_cast<float*>(bufVector[row])[col] = value;
}

void PAN_Satellite::normalize()
{
    if(bitsPerSample == 8 && sampleFormat == 1)
    {
        #pragma omp parallel for
        for (uint32_t row = 0; row < height; row++)
        {
            float* temp = new float[width];
            for(uint32_t x = 0; x < width; x++)
            {
                temp[x] = static_cast<float>(static_cast<uint8_t*>(bufVector[row])[x]);
            }

            delete[] bufVector[row];
            bufVector[row] = temp;
        }
    }
    else if(bitsPerSample == 16 && sampleFormat == 1)
    {
        #pragma omp parallel for
        for (uint32_t row = 0; row < height; row++)
        {
            float* temp = new float[width];
            for(uint32_t x = 0; x < width; x++)
            {
                temp[x] = static_cast<float>(static_cast<uint16_t*>(bufVector[row])[x]);
            }

            delete[] bufVector[row];
            bufVector[row] = temp;
        }
    }
    {
        Logger("Turn data into float");
    }

    stretch = new Stretch();
    stretch->SetType(SatelliteType::MUL);
    stretch->normalize_data(this);
}

PAN_Satellite::~PAN_Satellite()
{
    for(auto& buf : bufVector)
    {
        delete[] buf;
    }
    delete stretch;
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
    bufVector = std::vector<void*>(bands, nullptr);
}

void MUL_Satellite::read_in_buf()
{
    if(tile_length == 0 || tile_width == 0)
    {
        // read in lines
        if(planarConfig == 1)
        {
            {
                Logger("Planar Configuration is 1");
            }
            if(bitsPerSample == 8 && sampleFormat == 1)
            {
                #pragma omp parallel for
                for(uint32_t row = 0; row < height; row++)
                {
                    uint8_t* buf = new uint8_t[width * bands];
                    TIFFReadScanline(tif, buf, row);
                    for(uint16_t b = 0; b < bands; b++)
                    {
                        if (bufVector[b] == nullptr)
                        {
                            bufVector[b] = new uint8_t[height * width];
                        }
                        uint8_t* bandData = static_cast<uint8_t*>(bufVector[b]);
                        for(uint32_t x = 0; x < width; x++)
                        {
                            bandData[row * width + x] = buf[x * bands + b];
                        }
                    }
                    delete[] buf;
                }
                {
                    Logger("type is uint8");
                }
            }
            else if(bitsPerSample == 16 && sampleFormat == 1)
            {
                #pragma omp parallel for
                for(uint32_t row = 0; row < height; row++)
                {
                    uint16_t* buf = new uint16_t[width * bands];
                    TIFFReadScanline(tif, buf, row);
                    for(uint16_t b = 0; b < bands; b++)
                    {
                        if (bufVector[b] == nullptr)
                        {
                            bufVector[b] = new uint16_t[height * width];
                        }
                        uint16_t* bandData = static_cast<uint16_t*>(bufVector[b]);
                        for(uint32_t x = 0; x < width; x++)
                        {
                            bandData[row * width + x] = buf[x * bands + b];
                        }
                    }
                    delete[] buf;
                }
                {
                    Logger("type is uint16");
                }
            }
            else if(bitsPerSample == 32 && sampleFormat == 3)
            {
                #pragma omp parallel for
                for(uint32_t row = 0; row < height; row++)
                {
                    float* buf = new float[width * bands];
                    TIFFReadScanline(tif, buf, row);
                    for(uint16_t b = 0; b < bands; b++)
                    {
                        if (bufVector[b] == nullptr)
                        {
                            bufVector[b] = new float[height * width];
                        }
                        float* bandData = static_cast<float*>(bufVector[b]);
                        for(uint32_t x = 0; x < width; x++)
                        {
                            bandData[row * width + x] = buf[x * bands + b];
                        }
                    }
                    delete[] buf;
                }
                {
                    Logger("type is float32");
                }
            }
            else
            {
                Logger("Unsupported data type in MUL_Satellite::read_in_buf()");
                return;
            }
        }
        else if(planarConfig == 2)
        {
            {
                Logger("Planar Configuration is 2");
            }
            if(bitsPerSample == 8 && sampleFormat == 1)
            {
                for (uint16_t b = 0; b < bands; b++)
                {
                    if (bufVector[b] == nullptr)
                    {
                        bufVector[b] = new uint8_t[height * width];
                    }
                }
                #pragma omp parallel for
                for (uint32_t row = 0; row < height; row++)
                {
                    for (uint16_t b = 0; b < bands; b++)
                    {
                        uint8_t* bandData = static_cast<uint8_t*>(bufVector[b]);
                        TIFFReadScanline(tif, &bandData[row * width], row, b);
                    }
                }
                Logger("type is uint8");
            }
            else if(bitsPerSample == 16 && sampleFormat == 1)
            {
                for (uint16_t b = 0; b < bands; b++)
                {
                    if (bufVector[b] == nullptr)
                    {
                        bufVector[b] = new uint16_t[height * width];
                    }
                }
                #pragma omp parallel for
                for (uint32_t row = 0; row < height; row++)
                {
                    for (uint16_t b = 0; b < bands; b++)
                    {
                        uint16_t* bandData = static_cast<uint16_t*>(bufVector[b]);
                        TIFFReadScanline(tif, &bandData[row * width], row, b);
                    }
                }
                Logger("type is uint16");
            }
            else if(bitsPerSample == 32 && sampleFormat == 3)
            {
                for (uint16_t b = 0; b < bands; b++)
                {
                    if (bufVector[b] == nullptr)
                    {
                        bufVector[b] = new float[height * width];
                    }
                }
                #pragma omp parallel for
                for (uint32_t row = 0; row < height; row++)
                {
                    for (uint16_t b = 0; b < bands; b++)
                    {
                        float* bandData = static_cast<float*>(bufVector[b]);
                        TIFFReadScanline(tif, &bandData[row * width], row, b);
                    }
                }
                Logger("type is float32");
            }
            else
            {
                Logger("Unsupported data type in MUL_Satellite::read_in_buf()");
                return;
            }
        }
    }
    {
        Logger("read in data done");
    }
}

float MUL_Satellite::getPixelValue(int row, int col, int band)
{
    if (bufVector[band] != nullptr)
    {
        float* bandData = static_cast<float*>(bufVector[band]);
        return bandData[row * width + col];
    }
    return float();
}

void MUL_Satellite::setPixelValue(int row, int col, int band, float value)
{
    if (bufVector[band] != nullptr)
    {
        static_cast<float*>(bufVector[band])[row * width + col] = value;
    }
}

void MUL_Satellite::normalize()
{
    // turn the buf into float
    if(bitsPerSample == 8 && sampleFormat == 1)
    {
        for(uint16_t b = 0; b < bands; b++)
        {
            float* temp = new float[height * width];

            #pragma omp parallel for collapse(2)
            for(uint32_t row = 0; row < height; row++)
            {
                for(uint32_t x = 0; x < width; x++)
                {
                    temp[row * width + x] = static_cast<float>(static_cast<uint8_t*>(bufVector[b])[row * width + x]);
                }
            }

            delete[] bufVector[b];
            bufVector[b] = temp;
        }
    }
    else if(bitsPerSample == 16 && sampleFormat == 1)
    {
        for(uint16_t b = 0; b < bands; b++)
        {
            float* temp = new float[height * width];

            #pragma omp parallel for collapse(2)
            for(uint32_t row = 0; row < height; row++)
            {
                for(uint32_t x = 0; x < width; x++)
                {
                    temp[row * width + x] = static_cast<float>(static_cast<uint16_t*>(bufVector[b])[row * width + x]);
                }
            }

            delete[] bufVector[b];
            bufVector[b] = temp;
        }
    }
    {
        Logger("Turn data into float");
    }

    stretch = new Stretch();
    stretch->SetType(SatelliteType::MUL);
    stretch->normalize_data(this);
}

MUL_Satellite::~MUL_Satellite()
{
    for(auto& buf : bufVector)
    {
        delete[] buf;
    }
    delete stretch;
}