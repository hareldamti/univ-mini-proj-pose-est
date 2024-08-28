#pragma once
#include "Render.h"

class Mesh {
    public:
        Mesh();
        Mesh(std::vector<u32> indices, std::vector<f32> vertices);
        std::vector<u32> indices;
        std::vector<f32> vertices;
        void add(Mesh& newMesh, f32 u, f32 v);
        void add(Mesh& newMesh, glm::vec4& pos, glm::mat4& rot, f32 u, f32 v);
        static Mesh fromRoute(std::vector<Camera>& route);
};