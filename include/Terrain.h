#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include "stb_image.h"
#include "GraphicsUtils.h"
#include "utils.h"

class Terrain {
    private:
        int color_depth;

        
    public:
        Terrain(const char* filename, int downsize, float xWidth, float height);
        ~Terrain();
        float* vertices;
        unsigned* indices;
        float xWidth, yWidth, height;
        i32 texture;
        std::vector<float> vertices_vec;
        std::vector<unsigned int> indices_vec;
        std::vector<Triangle> triangles;

};