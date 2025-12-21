//Jiahao Zhou and Haojun Tang
//2025-2026
//git@github.com:Jeiluo/Fastview4Geo.git

#include <string>
#include <vector>
#include <map>
#include "GdalCommon.h"

// 图像元数据读取
class MetadataReader {
public:
    MetadataReader();
    ~MetadataReader();
    bool readMetadata(const std::string& imagePath);// 读取图像元数据
    std::vector<std::string> getBandInfo() const; // 获取波段信息
    std::string getSpatialReference() const;// 获取空间参考信息
    void getImageExtent(double& minX, double& minY, double& maxX, double& maxY) const;// 获取图像范围
    void getPixelSize(double& pixelSizeX, double& pixelSizeY) const;// 获取像素尺寸
    
    // 获取图像统计信息
    struct BandStats {
        double min;
        double max;
        double mean;
        double stddev;
    };

    BandStats getBandStats(int band) const;
    
private:
    void parseGdalMetadata(void* dataset);// 解析GDAL元数据
    void parseTiffTags();// 解析TIFF标签
private:
    GDALDataset* m_dataset = nullptr;
    std::map<std::string, std::string> m_metadata;
    std::vector<BandStats> m_bandStats;
    double m_geoTransform[6];
    std::string m_projection;
};