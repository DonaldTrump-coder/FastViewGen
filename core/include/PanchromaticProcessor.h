//Jiahao Zhou and Haojun Tang
//2025-2026
//git@github.com:Jeiluo/Fastview4Geo.git

#ifndef PANCHROMATICPROCESSOR_H
#define PANCHROMATICPROCESSOR_H

#include "ImageProcessor.h"
#include <opencv2/opencv.hpp>

//全色影像处理
class PanchromaticProcessor : public ImageProcessor {
    public:
    PanchromaticProcessor();
    ~PanchromaticProcessor() override = default;
    cv::Mat loadImage(const std::string& path) override;
    cv::Mat enhance(const cv::Mat& src, EnhancementMethod method) override;
    cv::Mat applyColorMap(const cv::Mat& src) override;
    cv::Mat generateOverview(const cv::Mat& src, const cv::Size& targetSize) override;
    ImageType getImageType() const override;
    
private:
    
    cv::Mat linearStretch(const cv::Mat& src);// 线性拉伸增强
    cv::Mat histogramEqualization(const cv::Mat& src);// 直方图均衡化增强
    cv::Mat histogramMatching(const cv::Mat& src, const cv::Mat& reference);// 直方图匹配增强
    cv::Mat adaptiveGammaCorrection(const cv::Mat& src);// 自适应伽马校正
    cv::Mat applyCLAHE(const cv::Mat& src);// 自适应直方图均衡化
    cv::Mat multiScaleRetinex(const cv::Mat& src);// Retinex增强算法
    cv::Mat detectHighlightRegions(const cv::Mat& src);// 检测高亮区域（云、雪）
    cv::Mat detectShadowRegions(const cv::Mat& src);// 检测阴影区域
    cv::Mat adjustDynamicRange(const cv::Mat& src);// 动态范围调整
private:
    cv::Mat m_referenceHistogram;  // 用于直方图匹配的参考直方图
    double m_cloudSnowThreshold;   // 云雪检测阈值
    double m_shadowThreshold;      // 阴影检测阈值
};

#endif // PANCHROMATICPROCESSOR_H