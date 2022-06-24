rmdir /s build
cmake -A x64 -B build . 
cmake --build ./build --config Release

rmdir /s build
cmake -A Win32 -B build . 
cmake --build ./build --config Release
