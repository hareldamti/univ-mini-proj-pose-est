#include "Mesh.h"

Mesh::Mesh() {}
Mesh::Mesh(std::vector<u32> indices, std::vector<f32> vertices) : indices(indices), vertices(vertices) {}
void Mesh::add(Mesh& newMesh, f32 u, f32 v) {
    u32 currLength = vertices.size() / 5;
    if (newMesh.vertices.size() % 3 != 0) ERROR_EXIT("Mesh with illegal vertices length loaded");
    for (i32 index : newMesh.indices) indices.push_back(index + currLength);
    for (int i = 0; i < newMesh.vertices.size(); i += 3) {
        glm::vec4 point(newMesh.vertices[i], newMesh.vertices[i + 1], newMesh.vertices[i + 2], 1);
        vertices.push_back(point.x);
        vertices.push_back(point.y);
        vertices.push_back(point.z);
        vertices.push_back(u);
        vertices.push_back(v);
    }
}
void Mesh::add(Mesh& newMesh, glm::vec4& pos, glm::mat4& rot, f32 u, f32 v) {
    u32 currLength = vertices.size() / 5;
    if (newMesh.vertices.size() % 3 != 0) ERROR_EXIT("Mesh with illegal vertices length loaded");
    for (i32 index : newMesh.indices) indices.push_back(index + currLength);
    for (int i = 0; i < newMesh.vertices.size(); i += 3) {
        glm::vec4 point(newMesh.vertices[i], newMesh.vertices[i + 1], newMesh.vertices[i + 2], 1);
        point = rot * point + pos;
        vertices.push_back(point.x);
        vertices.push_back(point.y);
        vertices.push_back(point.z);
        vertices.push_back(u);
        vertices.push_back(v);
    }
}

Mesh Mesh::fromRoute(std::vector<Camera>& route) {
    Mesh m;
    for (Camera& cam : route) {
        m.vertices.push_back(cam.pos.x);
        m.vertices.push_back(cam.pos.y);
        m.vertices.push_back(cam.pos.z);
    }
    for (int i = 0; i < route.size() - 1; i++) {
        m.indices.push_back(i);
        m.indices.push_back(i+1);
        //m.indices.push_back(i);
    }
    return m;
}