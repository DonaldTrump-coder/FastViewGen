//Jiahao Zhou and Haojun Tang
//2025-2026
//git@github.com:Jeiluo/Fastview4Geo.git

#include <string>
#include <map>
#include <opencv2/opencv.hpp>
#include "GdalCommon.h"

// 图像块管理
class TileManager {
public:
    TileManager();
    ~TileManager();
    bool initialize(const std::string& imagePath, int tileSize = 1024);// 初始化分块参数
    cv::Mat readTile(int tileX, int tileY);// 读取指定块
    bool writeTile(int tileX, int tileY, const cv::Mat& tile);// 写入指定块
    void getTileCount(int& tilesX, int& tilesY) const;// 获取块数量
    cv::Size getTileSize() const;// 获取块尺寸
    cv::Mat mergeAllTiles();// 合并所有块为完整图像
    
private:
    void calculateTileIndices();// 计算块索引
    // 管理块缓存
    cv::Mat getCachedTile(int tileX, int tileY);
    void cacheTile(int tileX, int tileY, const cv::Mat& tile);
private:
    std::string m_imagePath;
    cv::Size m_imageSize;
    cv::Size m_tileSize;
    int m_tilesX;
    int m_tilesY;
    std::map<std::pair<int, int>, cv::Mat> m_tileCache;
    int m_maxCacheSize;
    GDALDataset* m_dataset = nullptr;// GDAL 数据集
};