//Jiahao Zhou and Haojun Tang
//2025-2026
//git@github.com:Jeiluo/Fastview4Geo.git

#ifndef OVERVIEWGENERATOR_H
#define OVERVIEWGENERATOR_H

#include <string>
#include <vector>
#include <memory>
#include <opencv2/opencv.hpp>
#include "ImageProcessor.h"
#include "AdaptiveColorMapper.h"

//增强方法
enum class EnhancementMethod {
    LINEAR_STRETCH,                    //线性拉伸
    HISTOGRAM_EQUALIZATION,            //直方图均衡化
    HISTOGRAM_MATCHING                 //直方图匹配
};
//图形方法
enum class ImageType {
    PANCHROMATIC,                      //全色
    RGB,                               //RGB
    MULTISPECTRAL,                     //多光谱
    UNKNOWN
};

// 快视图生成
class OverviewGenerator {
public:
    OverviewGenerator();
    ~OverviewGenerator();

    void setEnhancementMethod(EnhancementMethod method);// 设置增强方法
    void setTargetSize(const cv::Size& size);// 设置目标尺寸
    void setOutputPath(const std::string& path);// 设置输出路径
    bool generate(const std::string& imagePath);// 生成快视图
    bool batchGenerate(const std::vector<std::string>& imagePaths);// 批量生成
    ImageType detectImageType(const std::string& imagePath);// 获取图像类型

    struct PerformanceStats {
        double loadTime;
        double enhanceTime;
        double colorMapTime;
        double resizeTime;
        double totalTime;
    };

    PerformanceStats getLastPerformanceStats() const;

private:
    std::unique_ptr<ImageProcessor> createProcessor(ImageType type);// 创建处理器
    bool validateInput(const std::string& imagePath);// 验证输入
    bool saveResult(const cv::Mat& image, const std::string& baseName);// 保存结果
    void logMessage(const std::string& message, int level = 0);// 日志记录

private:
    std::unique_ptr<ImageProcessor> m_processor;
    AdaptiveColorMapper m_colorMapper;
    EnhancementMethod m_enhancementMethod;
    cv::Size m_targetSize;
    std::string m_outputPath;
    PerformanceStats m_stats;
    bool m_enableParallel;
};

#endif // OVERVIEWGENERATOR_H