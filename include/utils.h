#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <math.h>
#include <map>
#include <format>
#include <iostream>
#include "glad/glad.h"
#include <gl\gl.h>	
#include <gl\glu.h>	
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/gtc/type_ptr.hpp>
#include <inttypes.h>
#include "opencv2/calib3d/calib3d.hpp"
#include <sys/timeb.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float f32;
typedef double f64;

#define PI 3.141592653589793f

#define ERROR_EXIT(...) { fprintf(stderr, __VA_ARGS__); exit(1); }
#define ERROR_RETURN(R, ...) { fprintf(stderr, __VA_ARGS__); return R; }
#define LOG_DEBUG(...) { fprintf(stderr,"DEBUG:\t"); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); }
#define LOG_ERROR(...) { fprintf(stderr,"ERROR:\t"); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); }
#define GLCall(f) {_glClearError();\
    f;\
    _glCatchError(#f, __FILE__, __LINE__);}
#define GLSet(x,f) {_glClearError();\
    x = f;\
    _glCatchError(#f, __FILE__, __LINE__);}

void _glClearError();
bool _glCatchError(const char* func_name, const char* file, int line);
std::string readFile(const std::string& file_path);
void loadParametersFile();
f32 getParam(std::string name);

cv::Point2f Point2f(glm::vec2& point);
cv::Point3f Point3f(glm::vec3& point);
glm::mat4 Mat4(cv::Mat& mat);
glm::vec4 Vec4(cv::Mat& tvec);
glm::vec4 Vec4(cv::Point3f& point);

std::string format(glm::mat4 m);
std::string format(glm::vec4 m);

class Params {
    public:
        std::string filename;
        Params(std::string filename);
        std::map<std::string, f32> params;
        f32 get(std::string name);
        f32 getDebug(std::string name);
};