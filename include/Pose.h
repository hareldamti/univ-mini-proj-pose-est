#pragma once
#include "Terrain.h"
#include "Render.h"
#include <cmath>

struct Intersection {
    bool hit;
    float distance;
    glm::vec3 point;
    int triangleIndex;
};

class Pose {
    public:
        static void printIntersection(const Intersection& intersection);
        static Intersection cast(Camera cam, cv::Point2f screen, Terrain& terrain, Render& terrainRenderer);
};