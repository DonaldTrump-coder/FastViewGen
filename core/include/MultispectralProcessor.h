//Jiahao Zhou, Haojun Tang
//2025-2026
//git@github.com:Jeiluo/Fastview4Geo.git

//多光谱影像处理类，继承自 ImageProcessor
//波段合成、白平衡、色彩增强、直方图匹配等方法
//保存波段顺序和色彩参考

#ifndef MULTISPECTRALPROCESSOR_H
#define MULTISPECTRALPROCESSOR_H

#include "ImageProcessor.h"
#include <vector>

// 多光谱影像处理
class MultispectralProcessor : public ImageProcessor {
public:
    MultispectralProcessor();
    ~MultispectralProcessor() override = default;
    
    //cv::Mat loadImage(const std::string& path) override;
    //cv::Mat enhance(const cv::Mat& src, EnhancementMethod method) override;
    //cv::Mat applyColorMap(const cv::Mat& src) override;
    //cv::Mat generateOverview(const cv::Mat& src, const cv::Size& targetSize) override;
    ImageType getImageType() const override;
    
private:
    //cv::Mat bandComposition(const cv::Mat& src);// 波段合成（RGB合成）
    //cv::Mat autoWhiteBalance(const cv::Mat& src);// 自动白平衡(灰度世界法)
    //cv::Mat perfectReflectorWhiteBalance(const cv::Mat& src);// 自动白平衡（完美反射体法）
    //cv::Mat enhanceSaturation(const cv::Mat& src);// 色彩饱和度增强（HSV）
    //cv::Mat enhanceSaturationLab(const cv::Mat& src);// 色彩饱和度增强（Lab）
    //cv::Mat channelHistogramMatching(const cv::Mat& src);// 直方图匹配（分通道）
    //cv::Mat colorConsistencyCorrection(const cv::Mat& src);// 色彩一致性校正
    //double computeColorEntropy(const cv::Mat& src);// 计算色彩熵

private:
    std::vector<int> m_bandOrder;      // 波段顺序 [B,G,R,NIR...]
    //cv::Mat m_colorReference;          // 色彩参考图像
    bool m_useHSV;                     // HSV or Lab
};

#endif // MULTISPECTRALPROCESSOR_H