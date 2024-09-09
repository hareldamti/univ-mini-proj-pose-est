OUT := pose-est.exe
OPENCV_DIR := C:/opencv/build/install
SRC := main.cpp src/*.cpp
VENDORS := vendors/src/*.c
INCLUDES := -Ivendors/include/ -Iinclude/ -I$(OPENCV_DIR)/include
LIBS := -L$(OPENCV_DIR)/x64/mingw/lib
DLLS := -lopengl32 -lglu32 -lgdi32 -llibopencv_core4100 -llibopencv_highgui4100 \
-llibopencv_imgproc4100 -llibopencv_imgcodecs4100 -llibopencv_calib3d4100
SUPPRESS := -Wno-deprecated-enum-enum-conversion
all: $(OUT)

$(OUT): $(SRC)
	g++ -std=c++20 -g $(INCLUDES) $(LIBS) -o $(OUT) $(SRC) $(VENDORS) $(DLLS) $(SUPPRESS)
