//Jiahao Zhou, Haojun Tang
//2025-2026
//git@github.com:Jeiluo/Fastview4Geo.git

//自适应颜色映射类,负责分析图像内容、选择颜色映射策略、创建自定义颜色表

#include <string>
#include <vector>
#include <map>
#include "OverviewGenerator.h"

// 自适应颜色映射
class AdaptiveColorMapper {
public:
    AdaptiveColorMapper();
    ~AdaptiveColorMapper() = default;
    //cv::Mat applyColorMap(const cv::Mat& src, ImageType type);// 应用颜色映射
    void setColorMapStrategy(int strategy);// 设置颜色映射策略
    std::vector<std::string> getAvailableColorMaps() const;// 获取可用的颜色映射列表
    
private:
    //int selectPanchromaticColorMap(const cv::Mat& src);// 为全色图像选择颜色映射
    //int selectMultispectralColorMap(const cv::Mat& src);// 为多光谱图像选择颜色映射
    //std::string analyzeImageContent(const cv::Mat& src);// 分析图像内容（雪地、海洋、阴影等）
    //cv::Mat createCustomColorMap(int mapType);// 创建自定义颜色映射表

private:
    int m_currentStrategy;             // 当前颜色映射策略
    //cv::Mat m_customColorMap;         // 自定义颜色映射表
    std::map<std::string, int> m_contentToColorMap; // 内容到颜色映射的映射
};