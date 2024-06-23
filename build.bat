g++ -g -o pose-est.exe ^
main.cpp vendors/src/*.c src/*.cpp ^
-Ivendors/include/ -Iinclude/ -IC:/opencv/build/include/ ^
-lopengl32 -lglu32 -lgdi32