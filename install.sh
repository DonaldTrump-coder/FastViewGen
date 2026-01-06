git clone https://github.com/Jeiluo/Fastview4Geo
git submodule update --
git submodule update --remote

# install libtiff
mkdir build
cd build
mkdir libtiff
cd libtiff
cmake ..\..\trd_party\libtiff\ -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
cmake --install . --prefix ./install/
cd ..

# install curl
mkdir curl
cd curl
cmake ..\..\trd_party\curl\ -DCMAKE_BUILD_TYPE=Release -DCURL_USE_LIBPSL=OFF
cmake --build . --config Release
cmake --install . --prefix ./install/
cd ..

# install PROJ
mkdir PROJ
cd PROJ
cmake -DBUILD_PROJSYNC=OFF ..\..\trd_party\PROJ\ -DSQLITE3_INCLUDE_DIR="$(Resolve-Path ../../build/sqlite/install/include)" -DSQLITE3_LIBRARY="$(Resolve-Path ../../build/sqlite/install/lib/sqlite3.lib)" -DEXE_SQLITE3="$(Resolve-Path ../../build/sqlite/install/bin/sqlite3.exe)" -DTIFF_INCLUDE_DIR="$(Resolve-Path ../../build/libtiff/install/include)" -DTIFF_LIBRARY="$(Resolve-Path ../../build/libtiff/install/lib/tiff.lib)" -DCURL_INCLUDE_DIR="$(Resolve-Path ../../build/curl/install/include)" -DCURL_LIBRARY="$(Resolve-Path ../../build/curl/install/lib/libcurl_imp.lib)"
cmake --build . --config Release
cmake --install . --prefix ./install/
cd ..

# install GDAL
mkdir gdal
cd gdal
cmake ..\..\trd_party\gdal\ -DPROJ_INCLUDE_DIR="$(Resolve-Path ../../build/PROJ/install/include)" -DPROJ_LIBRARY="$(Resolve-Path ../../build/PROJ/install/lib/proj.lib)"
cmake --build . --config Release
cmake --install . --prefix ./install/
cd ..