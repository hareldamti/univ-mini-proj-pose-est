#pragma once
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "glad.h"
#include <gl\gl.h>	
#include <gl\glu.h>	
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/gtc/type_ptr.hpp>

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