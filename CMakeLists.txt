cmake_minimum_required(VERSION 3.10)

project(Fastview4Geo)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create build directories for each external dependency
#CMAKE_BINARY_DIR: the project build DIR
set(BUILD_DIR ${CMAKE_BINARY_DIR})

set(LIBTIFF_INSTALL_DIR ${BUILD_DIR}/libtiff/install)

set(YAMLCPP_INSTALL_DIR ${BUILD_DIR}/yaml_cpp/install)

set(STBIMAGE_DIR ${CMAKE_SOURCE_DIR}/trd_party/stb)

set(LIBTIFF_DLL_DIR ${LIBTIFF_INSTALL_DIR}/bin)

add_executable(Fastview4Geo
               core/src/main.cpp
               core/src/Reader.cpp
               core/src/Logger.cpp
               core/src/Satellite.cpp
               core/src/Stretch.cpp
              )

# Add include directories for each external library
include_directories(
    ${LIBTIFF_INSTALL_DIR}/include
    ${CMAKE_SOURCE_DIR}/core/include
    ${YAMLCPP_INSTALL_DIR}/include
    ${STBIMAGE_DIR}
)

# Link the external libraries to the executable
find_package(TIFF REQUIRED PATHS ${LIBTIFF_INSTALL_DIR}/lib/cmake/tiff)
find_package(yaml-cpp REQUIRED PATHS ${YAMLCPP_INSTALL_DIR}/lib/cmake/yaml-cpp)
target_link_libraries(Fastview4Geo 
    ${LIBTIFF_INSTALL_DIR}/lib/tiff.lib
    ${LIBTIFF_INSTALL_DIR}/lib/tiffxx.lib
    ${YAMLCPP_INSTALL_DIR}/lib/yaml-cpp.lib
)

add_custom_command(TARGET Fastview4Geo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${LIBTIFF_DLL_DIR}/tiff.dll
    $<TARGET_FILE_DIR:Fastview4Geo>/tiff.dll)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)