cmake_minimum_required(VERSION 3.10)

project(Fastview4Geo)

# Create build directories for each external dependency
#CMAKE_BINARY_DIR: the project build DIR
set(BUILD_DIR ${CMAKE_BINARY_DIR})

set(LIBTIFF_INSTALL_DIR ${BUILD_DIR}/libtiff/install)

set(CURL_INSTALL_DIR ${BUILD_DIR}/curl/install)

set(PROJ_INSTALL_DIR ${BUILD_DIR}/PROJ/install)

set(GDAL_INSTALL_DIR ${BUILD_DIR}/gdal/install)

add_executable(Fastview4Geo core/src/main.cpp)

# Add include directories for each external library
target_include_directories(Fastview4Geo PRIVATE
    ${LIBTIFF_INSTALL_DIR}/include
    ${CURL_INSTALL_DIR}/include
    ${PROJ_INSTALL_DIR}/include
    ${GDAL_INSTALL_DIR}/include
    ${CMAKE_SOURCE_DIR}/core/include
)

# Link the external libraries to the executable
target_link_libraries(Fastview4Geo 
    ${LIBTIFF_INSTALL_DIR}/lib/tiff.lib
    ${CURL_INSTALL_DIR}/lib/libcurl_imp.lib
    ${PROJ_INSTALL_DIR}/lib/proj.lib
    ${GDAL_INSTALL_DIR}/lib/gdal.lib
)