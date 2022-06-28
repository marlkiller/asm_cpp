rm -rf build
cmake . -B build
make -C build
cd ./build
./bin_asm_cpp