git config --global core.longpaths true
git submodule sync
git submodule update --init
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