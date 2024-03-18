rm -rf build
mkdir -p build
cd build
cmake ..
#cmake -DCMAKE_BUILD_TYPE=Debug ..
make
cd ..