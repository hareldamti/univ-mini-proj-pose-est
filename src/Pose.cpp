#include "Pose.h"

Intersection computeIntersection(glm::vec4 src, glm::vec4 dir, Triangle triangle, int index) {
    glm::vec3 u = triangle.b - triangle.a,
              v = triangle.c - triangle.a,
              r = glm::vec3(dir);
    
    glm::mat3 intersectionMatrix = glm::mat3(u, v, dir);
    float d = glm::determinant(intersectionMatrix);
    if (glm::abs(d) < 10e-4f) return {false, -1, {}};
    glm::vec3 components;
    for (int i = 0; i < 3; i++) {
        glm::mat3 m(intersectionMatrix);
        m[i] = glm::vec3(src) - triangle.a;
        components[i] = glm::determinant(m) / d;
    }

    if (components.x < 0 || components.y < 0 || components.x + components.y > 1 || components.z < 0) return {false, -1, {}};
    return {true, components.z, triangle.a + components.x * u + components.y * v};
}

Intersection Pose::cast(Camera cam, cv::Point2f screen, Terrain& terrain, Render& terrainRenderer)
{
    /// TODO: Change ratio based on renderer's ratio
    float   ratio = terrainRenderer.width * 1.0 / terrainRenderer.height,
            fov = glm::tan(CAMERA_FOV * glm::pi<float>() / 277);
    glm::vec4 ray = glm::normalize(glm::vec4(
        (1 - 2*screen.x) * fov * ratio,
        (2*screen.y - 1) * fov,
        1,
        1
    ));
    ray = cam.rot * ray;
    
    Intersection closestIntersection = {false, -1, {}};
    int index = 0;
    for (auto& triangle : terrain.triangles) {
        Intersection intersectionPoint = computeIntersection(cam.pos, ray, triangle, index);
        if (!intersectionPoint.hit) continue;
        if (closestIntersection.hit && closestIntersection.distance < intersectionPoint.distance) continue;
        closestIntersection = intersectionPoint;
        closestIntersection.triangleIndex = index;
        index += 1;
    }

    return closestIntersection;
}

void Pose::printIntersection(const Intersection& intersection) {
    if (intersection.hit) {
        std::cout << "Intersection found:\n";
        std::cout << "  Triangle Index: " << intersection.triangleIndex << "\n";
        std::cout << "  Distance: " << intersection.distance << "\n";
        std::cout << "  Intersection Point: ("
                  << intersection.point.x << ", "
                  << intersection.point.y << ", "
                  << intersection.point.z << ")\n";
    } else {
        std::cout << "No intersection found.\n";
    }
}