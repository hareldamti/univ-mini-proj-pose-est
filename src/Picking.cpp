#include "Picking.h"


Intersection computeIntersection(glm::vec4 src, glm::vec4 dir, Triangle triangle, int index){
    glm::vec3 u = triangle.b - triangle.a,
              v = triangle.c - triangle.a,
              r = glm::vec3(dir);
    
    glm::mat3 intersectionMatrix = glm::transpose(glm::mat3(u, v, dir));
    if (glm::abs(glm::determinant(intersectionMatrix)) < 10e-4f) return {false, -1, {}};
    glm::vec3 components = glm::inverse(intersectionMatrix) * (glm::vec3(src) - triangle.a);
    LOG_DEBUG("components (%f %f %f)", components.x, components.y, components.z);
    if (components.x < 0 || components.y < 0 || components.x + components.y > 1 || components.z < 0) return {false, -1, {}};
    return {true, components.z, triangle.a + components.x * u + components.y * v};
}

Intersection Picking::cast(glm::vec4& cameraPos, glm::mat4& cameraRot, Render& terrain_renderer, State& state, Terrain& terrain)
{
    /// TODO: Change ratio based on renderer's ratio
    float   u = state.input.mouseX * 1.0 / state.window.width,
            v = state.input.mouseY * 1.0 / state.window.height,
            ratio = state.window.width * 1.0 / state.window.height,
            fov = glm::tan(CAMERA_FOV * glm::pi<float>() / 360);
    glm::vec4 ray = glm::normalize(glm::vec4(
        (2*u - 1) * fov * ratio,
        (2*v - 1) * fov,
        1,
        1
    ));
    ray = cameraRot * ray;
    Intersection closestIntersection = {false, -1, {}};
    int index = 0;
    for (auto& triangle : terrain.triangles) {
        Intersection intersectionPoint = computeIntersection(cameraPos, ray, triangle, index);
        if (!intersectionPoint.hit) continue;
        if (closestIntersection.hit && closestIntersection.distance < intersectionPoint.distance) continue;
        closestIntersection = intersectionPoint;
        closestIntersection.triangleIndex = index;
        index += 1;
    }

    return closestIntersection;
}



// Ray Picking::generateRayFromScreenCoordinates(Vec3 camera_pos, float screenX, float screenY, float screenWidth, float screenHeight) {
//      // Convert screen coordinates to normalized device coordinates (NDC)
//     float ndcX = (2.0f * screenX) / screenWidth - 1.0f;
//     float ndcY = 1.0f - (2.0f * screenY) / screenHeight;
    
//     Vec3 rayDir = {ndcX, ndcY, -1.0f};
//     rayDir = rayDir.normalize(); // Normalize the ray direction
    
//     return {camera_pos, rayDir};
// }

// bool Picking::rayIntersectsTriangle(const Ray& ray, const Triangle& triangle, float& t, Vec3& intersectionPoint) {
//     const float EPSILON = 0.0000001;
//     Vec3 edge1 = triangle.b - triangle.a;
//     Vec3 edge2 = triangle.c - triangle.a;
//     Vec3 h = ray.direction.cross(edge2);
//     float a = edge1.dot(h);
//     if (a > -EPSILON && a < EPSILON)
//         return false;
//     float f = 1.0 / a;
//     Vec3 s = ray.origin - triangle.a;
//     float u = f * s.dot(h);
//     if (u < 0.0 || u > 1.0)
//         return false;
//     Vec3 q = s.cross(edge1);
//     float v = f * ray.direction.dot(q);
//     if (v < 0.0 || u + v > 1.0)
//         return false;
//     t = f * edge2.dot(q);
//     if (t > EPSILON) {
//         intersectionPoint = ray.origin + ray.direction * t;
//         return true;
//     } else
//         return false;
// }


// Intersection Picking::pickAboo(Vec3 camera_pos, float screenX, float screenY, float screenWidth, float screenHeight, Terrain terrain){

//     Ray ray = generateRayFromScreenCoordinates(camera_pos, screenX, screenY, screenWidth, screenHeight);

//     Intersection closestIntersection = {false, -1, {}, -1};

//     // compute intersection for all the triangles, and saving the closest one

//     for (int i = 0; i < terrain.triangles.size(); i++) {
//     float t;
//     Vec3 intersectionPoint;
//     if (rayIntersectsTriangle(ray, terrain.triangles[i], t, intersectionPoint) && t < closestIntersection.distance) {
//         closestIntersection.hit = true;
//         closestIntersection.distance = t;
//         closestIntersection.point = intersectionPoint;
//         closestIntersection.triangleIndex = i;
//         }
//     }
    
//     return closestIntersection;

// }

void Picking::printIntersection(const Intersection& intersection) {
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