# FastViewGener: Fast Overviews Generator for Large Satellite Images

[![GitHub Repo stars](https://img.shields.io/github/stars/Jeiluo/Fastview4Geo?style=social)](https://github.com/Jeiluo/Fastview4Geo)  
[![GitHub Repo forks](https://img.shields.io/github/forks/Jeiluo/Fastview4Geo?style=social)](https://github.com/Jeiluo/Fastview4Geo)  
[![License: Apache 2.0](https://img.shields.io/badge/License-Apache_2.0-blue.svg?style=flat)](http://www.apache.org/licenses/)  
![Last Commit](https://img.shields.io/github/last-commit/Jeiluo/Fastview4Geo?color=green)

A project that generates fast overviews for **PANCHROMATIC** and **MULTISPECTRAL** satellite images of a maximum size of 4GB with adaptive color adjustment.

Contributor:[Haojun Tang](https://donaldtrump-coder.github.io/), [Jiahao Zhou](https://github.com/Jeiluo)

---

## Usage
**In config file:**<br>
```
config.yaml
├── data
│   ├── filename: string
│   └── stretch: enum
│          - linear
│          - log
│          - gamma
│          - tile_stretch
│          - piecewise_equal
│          - histo_equal
└── output
```

## Reuslts
| Stretch Methods | Linear | Log | Gamma | Tile Stretch | Piecewise Equal | Histogram Equal |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| Sat1 | <img src="./res/Sat1_linear.png" width=100% height = 100%/> |   |   |   |   |   |
| Sat2 |   |   |   |   | <img src="./res/Sat2_piecewise_equal.png" width=100% height = 100%/> | <img src="./res/Sat2_histo_equal.png" width=100% height = 100%/> |
| Sat3 | <img src="./res/Sat3_linear.png" width=100% height = 100%/> |   |   |   |   |   |
| Sat4 |   |   |   |   |   |   |
| Sat5 |   |   |   |   |   |   |
| Sat7 |   |   |   |   | <img src="./res/Sat7_piecewise_equal.png" width=100% height = 100%/> |   |

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

## 单独编译Yaml-cpp
cd .\build\
mkdir yaml_cpp
cd yaml_cpp
cmake ..\..\trd_party\yaml_cpp\ -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
cmake --install . --prefix ./install/
cd..

## 直接编译
```
.\install.ps1
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --clean-first --config Release
cmake --install .
./Release/Fastview4Geo.exe
```