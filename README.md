# Fastview4Geo — Fast Overview Generator for Large Remote Sensing Images  
# Fastview4Geo — 大规模遥感影像快视图生成器

[![GitHub Repo stars](https://img.shields.io/github/stars/Jeiluo/Fastview4Geo?style=social)](https://github.com/Jeiluo/Fastview4Geo)  
[![GitHub Repo forks](https://img.shields.io/github/forks/Jeiluo/Fastview4Geo?style=social)](https://github.com/Jeiluo/Fastview4Geo)  
[![License: Apache 2.0](https://img.shields.io/badge/License-Apache_2.0-blue.svg?style=flat)](http://www.apache.org/licenses/)  
![Last Commit](https://img.shields.io/github/last-commit/Jeiluo/Fastview4Geo?color=green)

A project to generate fast overviews for **panchromatic and multispectral images** of 4GB and above with adaptive color adjustment.  
This library supports **C++** usage, successfully tested on **Windows 10 and Windows 11**.  

一个用于生成 4GB 及以上**全色与多光谱影像快视图**的项目，具备自适应颜色调整功能。  
该库支持 **C++**，已在 **Windows 10 和 Windows 11** 下测试通过。  

Contributor: [Jeiluo Smith](https://github.com/Jeiluo), [Haojun Tang](https://donaldtrump-coder.github.io/)

---

## About / 关于项目

### Environment / 环境
<img src="./resources/environments.png" width="55%" />

### Structure / 项目结构
<img src="./resources/structure.png" width="55%" />

---

## Core Classes and Workflow / 核心类与处理流程

For large images (e.g., 4–6GB), processing typically involves the following classes:  
处理大体量影像（4–6GB）时，典型处理流程涉及以下类：

1. **GdalInitializer / GDAL 初始化器**  
   - Initializes the GDAL library for geospatial image reading/writing.  
   - 初始化 GDAL 库，用于读取和写入遥感影像数据。

2. **MetadataReader / 元数据读取器**  
   - Reads image metadata: bands, spatial reference, pixel size, image extent, statistics.  
   - 读取影像元数据：波段信息、空间参考、像素尺寸、影像范围及统计信息。

3. **TileManager / 分块管理器**  
   - Handles block-based reading/writing and caching to support large images.  
   - 管理影像分块读取/写入和缓存，保证大图处理时内存安全。

4. **ImageProcessor / 图像处理器 (抽象类)**  
   - Defines interface: `loadImage`, `enhance`, `applyColorMap`, `generateOverview`, `getImageType`.  
   - 定义通用接口：`loadImage`、`enhance`、`applyColorMap`、`generateOverview`、`getImageType`。

5. **PanchromaticProcessor / MultispectralProcessor / 全色/多光谱处理器**  
   - Implements type-specific processing:  
     - Panchromatic: Retinex, CLAHE, linear stretch, highlight/shadow detection.  
     - Multispectral: band composition, white balance, color enhancement, histogram matching.  
   - 实现影像类型特定处理：  
     - 全色：Retinex、CLAHE、线性拉伸、高光/阴影检测  
     - 多光谱：波段合成、白平衡、色彩增强、直方图匹配

6. **AdaptiveColorMapper / 自适应颜色映射器**  
   - Selects color mapping based on image content (cloud, snow, shadow).  
   - 根据影像内容（云、雪、阴影）自动选择颜色映射策略。

7. **OverviewGenerator / 快视图生成器**  
   - Generates thumbnails or fast views.  
   - Supports batch processing and records performance statistics.  
   - 生成缩略图或快视图，支持批量处理，并记录性能数据。

---

## Build and Run (C++) / 编译与运行 (C++)

### 1. Install MinGW-w64 / 安装 MinGW-w64
Download [MinGW-w64 v13](https://github.com/niXman/mingw-builds-binaries/releases/download/15.2.0-rt_v13-rev0/x86_64-15.2.0-release-win32-seh-ucrt-rt_v13-rev0.7z), unzip, and add `mingw64\bin` to PATH.  
Download [git]
下载 [MinGW-w64 v13](https://github.com/niXman/mingw-builds-binaries/releases/download/15.2.0-rt_v13-rev0/x86_64-15.2.0-release-win32-seh-ucrt-rt_v13-rev0.7z)，解压后将 `mingw64\bin` 加入环境变量 PATH。  
下载 [git]

Verify:S
```
cmd
git --version
gcc -v
g++ -v
```

## Cloning the Project
git clone --recursive https://github.com/Jeiluo/Fastview4Geo
or
git clone https://github.com/Jeiluo/Fastview4Geo
git submodule update --
git submodule update --remote

## 单独编译libtiff
mkdir build
cd build
mkdir libtiff
cd libtiff
cmake ..\..\trd_party\libtiff\ -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
cmake --install . --prefix ./install/
cd ..
编译至build/libtiff

## 单独编译curl
cd build
mkdir curl
cd curl
cmake ..\..\trd_party\curl\ -DCMAKE_BUILD_TYPE=Release -DCURL_USE_LIBPSL=OFF
cmake --build . --config Release
cmake --install . --prefix ./install/
cd ..

## 单独编译PROJ
cd .\build\
mkdir PROJ
cd PROJ
cmake -DBUILD_PROJSYNC=OFF ..\..\trd_party\PROJ\ -DSQLITE3_INCLUDE_DIR="$(Resolve-Path ../../build/sqlite/install/include)" -DSQLITE3_LIBRARY="$(Resolve-Path ../../build/sqlite/install/lib/sqlite3.lib)" -DEXE_SQLITE3="$(Resolve-Path ../../build/sqlite/install/bin/sqlite3.exe)" -DTIFF_INCLUDE_DIR="$(Resolve-Path ../../build/libtiff/install/include)" -DTIFF_LIBRARY="$(Resolve-Path ../../build/libtiff/install/lib/tiff.lib)" -DCURL_INCLUDE_DIR="$(Resolve-Path ../../build/curl/install/include)" -DCURL_LIBRARY="$(Resolve-Path ../../build/curl/install/lib/libcurl_imp.lib)"
cmake --build . --config Release
cmake --install . --prefix ./install/
cd..

## 单独编译GDAL
cd .\build\
mkdir gdal
cd gdal
cmake ..\..\trd_party\gdal\ -DPROJ_INCLUDE_DIR="$(Resolve-Path ../../build/PROJ/install/include)" -DPROJ_LIBRARY="$(Resolve-Path ../../build/PROJ/install/lib/proj.lib)"
cmake --build . --config Release
cmake --install . --prefix ./install/
cd..


```
.\install.sh
cd build
cmake ..
cmake --build .
cmake --install .
```