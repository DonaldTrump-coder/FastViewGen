//Jiahao Zhou, Haojun Tang
//2025-2026
//git@github.com:Jeiluo/Fastview4Geo.git

//抽象基类 ImageProcessor
//定义图像处理接口（加载、增强、颜色映射、生成快视图、获取类型）
//包含 GDALDataset 指针，提供部分工具方法（resize、统计信息）

#include <string>
#include "OverviewGenerator.h"

// 图像处理
class ImageProcessor {
public:
    virtual ~ImageProcessor() = default;
    //virtual cv::Mat loadImage(const std::string& path) = 0;// 分块读取
    //virtual cv::Mat enhance(const cv::Mat& src, EnhancementMethod method) = 0;// 图像增强核心
    //virtual cv::Mat applyColorMap(const cv::Mat& src) = 0;// 应用颜色映射
    //virtual cv::Mat generateOverview(const cv::Mat& src, const cv::Size& targetSize) = 0;// 生成指定尺寸的快视图
    virtual ImageType getImageType() const = 0;// 获取图像类型
protected:
    //cv::Mat resizeImage(const cv::Mat& src, const cv::Size& targetSize);// 图像调整大小
    //void computeImageStats(const cv::Mat& src, double& minVal, double& maxVal, 
    //                      double& mean, double& stddev);  // 统计图像信息

    GDALDataset* m_dataset = nullptr;// GDAL 数据集指针
};