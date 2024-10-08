#include "Pose.h"

Intersection computeIntersection(glm::vec4 src, glm::vec4 dir, Triangle triangle, int index) {
    glm::vec3 u = triangle.b - triangle.a,
              v = triangle.c - triangle.a,
              r = glm::vec3(dir);
    
    glm::mat3 intersectionMatrix = glm::mat3(u, v, -dir);
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

cv::Point2f Pose::projectToScreen(Camera& cam, cv::Point3f& pos, Render& terrainRenderer) {
    float ratio = terrainRenderer.width * 1.0 / terrainRenderer.height,
            fov = -glm::tan(CAMERA_FOV / 2);

    glm::mat4 _cam = -glm::transpose(cam.rot);
    _cam[3][3] = 1;
    _cam = glm::translate(_cam, -glm::vec3(cam.pos));
    glm::vec4 _p = glm::perspective(
        CAMERA_FOV,
        ratio,
        0.1f, 1000.0f
        ) * _cam * Vec4(pos);
    
    cv::Point2f _point = cv::Point2f(_p.x/_p.w, _p.y/_p.w);

    return _point;
}

Intersection Pose::cast(Camera cam, cv::Point2f screen, Terrain& terrain, Render& terrainRenderer)
{
    /// TODO: Change ratio based on renderer's ratio
    float   ratio = terrainRenderer.width * 1.0 / terrainRenderer.height,
            fov = -glm::tan(CAMERA_FOV / 2);
    glm::vec4 ray = glm::normalize(
        glm::vec4(
        screen.x * fov * ratio,
        screen.y * fov,
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

Camera Pose::solvePnP(std::vector<cv::Point3f>& points, std::vector<cv::Point2f>& screen, Render& terrainRenderer) {
    float fov = -glm::tan(CAMERA_FOV / 2 * terrainRenderer.m_state.params.getDebug("fov-correction"));
    cv::Mat cameraMatrix = (cv::Mat_<float>(3, 3) <<
        1.0/fov,    0,          0,   // fx, skew, cx
        0,          1.0/fov,    0,   // 0, fy, cy
        0,          0,          1    // 0, 0, 1
    );
    cv::Mat distCoeffs = cv::Mat::zeros(5, 1, CV_64F);
    cv::Mat rvec;
    cv::Mat tvec;
    LOG_DEBUG("solvePnP %d %d", points.size(), screen.size());
    if (!cv::solvePnP(points, screen, cameraMatrix, distCoeffs, rvec, tvec)) {
        LOG_DEBUG("solvePnP failed");
        return {glm::vec4(0),glm::mat4(-1)};
    };
    cv::Mat rotRod;
    cv::Rodrigues(rvec, rotRod);
    glm::vec4 pos = Vec4(tvec);
    glm::mat4 rot = Mat4(rotRod);

    return {-glm::transpose(rot) * pos, glm::transpose(rot)};
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

