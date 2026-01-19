cmake_minimum_required(VERSION 3.10)

project(Fastview4Geo)

# Create build directories for each external dependency
#CMAKE_BINARY_DIR: the project build DIR
set(BUILD_DIR ${CMAKE_BINARY_DIR})

set(LIBTIFF_INSTALL_DIR ${BUILD_DIR}/libtiff/install)

set(CURL_INSTALL_DIR ${BUILD_DIR}/curl/install)

set(PROJ_INSTALL_DIR ${BUILD_DIR}/PROJ/install)

set(GDAL_INSTALL_DIR ${BUILD_DIR}/gdal/install)

set(LIBTIFF_DLL_DIR ${LIBTIFF_INSTALL_DIR}/bin)

add_executable(Fastview4Geo
               core/src/main.cpp
               core/src/Reader.cpp
               core/src/Logger.cpp
              )

# Add include directories for each external library
include_directories(
    ${LIBTIFF_INSTALL_DIR}/include
    ${CMAKE_SOURCE_DIR}/core/include
)

# Link the external libraries to the executable
find_package(TIFF REQUIRED PATHS ${LIBTIFF_INSTALL_DIR}/lib/cmake/tiff)
target_link_libraries(Fastview4Geo 
    ${LIBTIFF_INSTALL_DIR}/lib/tiff.lib
    ${LIBTIFF_INSTALL_DIR}/lib/tiffxx.lib
)

add_custom_command(TARGET Fastview4Geo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${LIBTIFF_DLL_DIR}/tiff.dll
    $<TARGET_FILE_DIR:Fastview4Geo>/tiff.dll)