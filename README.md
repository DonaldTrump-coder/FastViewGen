# FastViewGener: Fast Overviews Generator for Large Satellite Images

[![GitHub Repo stars](https://img.shields.io/github/stars/DonaldTrump-coder/FastViewGen?style=social)](https://github.com/DonaldTrump-coder/FastViewGen)  
[![GitHub Repo forks](https://img.shields.io/github/forks/DonaldTrump-coder/FastViewGen?style=social)](https://github.com/DonaldTrump-coder/FastViewGen)  
[![License: Apache 2.0](https://img.shields.io/badge/License-Apache_2.0-purple.svg?style=flat)](http://www.apache.org/licenses/)  
![Last Commit](https://img.shields.io/github/last-commit/DonaldTrump-coder/FastViewGen?color=green)

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
| Stretch Methods | &nbsp;&nbsp;&nbsp;Linear&nbsp;&nbsp;&nbsp; | &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Log&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; | &nbsp;&nbsp;Gamma&nbsp;&nbsp; | Tile &nbsp;&nbsp;&nbsp;Stretch&nbsp;&nbsp;&nbsp; | Piecewise Equal | Histogram Equal |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| Sat1 | <img src="./res/Sat1_linear.png" width=100%> | <img src="./res/Sat1_log.png" width=100%> | <img src="./res/Sat1_gamma.png" width=100%/> | <img src="./res/Sat1_tile_stretch.png" width=100%/> | <img src="./res/Sat3_piecewise_equal.png" width=100%/> | <img src="./res/Sat1_histo_equal.png" width=100%/> |
| Sat2 | <img src="./res/Sat2_linear.png" width=100%> | <img src="./res/Sat2_log.png" width=100%> | <img src="./res/Sat2_gamma.png" width=100%/> | <img src="./res/Sat2_tile_stretch.png" width=100%/> | <img src="./res/Sat2_piecewise_equal.png" width=100%> | <img src="./res/Sat2_histo_equal.png" width=100%/> |
| Sat3 | <img src="./res/Sat3_linear.png" width=100%/> | <img src="./res/Sat3_log.png" width=100%/> | <img src="./res/Sat3_gamma.png" width=100%/> | <img src="./res/Sat3_tile_stretch.png" width=100%/> | <img src="./res/Sat3_piecewise_equal.png" width=100%/> | <img src="./res/Sat3_histo_equal.png" width=100%/> |
| Sat4 | <img src="./res/Sat4_linear.png" width=100%/> | <img src="./res/Sat4_log.png" width=100%/> | <img src="./res/Sat4_gamma.png" width=100%/> | <img src="./res/Sat4_tile_stretch.png" width=100%/> | <img src="./res/Sat4_piecewise_equal.png" width=100%/> | <img src="./res/Sat4_histo_equal.png" width=100%/> |
| Sat5 | <img src="./res/Sat5_linear.png" width=100%/> | <img src="./res/Sat5_log.png" width=100%/> | <img src="./res/Sat5_gamma.png" width=100%/> | <img src="./res/Sat5_tile_stretch.png" width=100%/> | <img src="./res/Sat5_piecewise_equal.png" width=100%/> | <img src="./res/Sat5_histo_equal.png" width=100%/> |
| Sat7 | <img src="./res/Sat7_linear.png" width=100%/> | <img src="./res/Sat7_log.png" width=100%/> | <img src="./res/Sat7_gamma.png" width=100%/> | <img src="./res/Sat7_tile_stretch.png" width=100%/> | <img src="./res/Sat7_piecewise_equal.png" width=100%/> | <img src="./res/Sat7_histo_equal.png" width=100%/> |

## Cloning the Project
```
git clone --recursive https://github.com/Jeiluo/Fastview4Geo
```
or
```
git clone https://github.com/Jeiluo/Fastview4Geo
git submodule update --
git submodule update --remote
```

## Build the whole project
For Windows: (in PowerShell)
```
.\install.ps1
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --clean-first --config Release
cmake --install .
./Release/Fastview4Geo.exe
```

## Only build submodules
Build for libtiff:
```
mkdir build
cd build
mkdir libtiff
cd libtiff
cmake ..\..\trd_party\libtiff\ -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
cmake --install . --prefix ./install/
cd ..
```
The module is build into path **./build/libtiff**.<br>
<br>
Build for Yaml-cpp:
```
cd build
mkdir yaml_cpp
cd yaml_cpp
cmake ..\..\trd_party\yaml_cpp\ -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
cmake --install . --prefix ./install/
cd..
```
The module is build into path **./build/yaml_cpp**.<br>
<br>