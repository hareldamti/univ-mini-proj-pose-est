#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include "stb_image.h"
#include "GraphicsUtils.h"

#define Z_SIZE 1.0
#define X_SIZE 5.0


class Terrain {
    private:

        float Y_SIZE;
        int color_depth;
        //std::vector<Triangle> triangles;
        
    public:

        Terrain(const char* filename, int downsize);
        ~Terrain();
        
        float* vertices;
        unsigned* indices;
        std::vector<float> vertices_vec;
        std::vector<unsigned int> indices_vec;
        std::vector<Triangle> triangles;

};