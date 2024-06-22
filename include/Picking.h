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

class Picking {
    private:
    static Ray generateRayFromScreenCoordinates(Vec3 camera_pos, float screenX, float screenY, float screenWidth, float screenHeight);
    static bool rayIntersectsTriangle(const Ray& ray, const Triangle& triangle, float& t, Vec3& intersectionPoint);

    public:
    static Intersection pickAboo(Vec3 camera_pos, float screenX, float screenY, float screenWidth, float screenHeight, Terrain terrain);
    static void printIntersection(const Intersection& intersection);
    static Intersection cast(glm::vec4& cameraPos, glm::mat4& cameraRot, Render& terrain_renderer, State& state, Terrain& terrain);
};