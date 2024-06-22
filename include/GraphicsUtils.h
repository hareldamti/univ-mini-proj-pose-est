#pragma once
#include <cmath> 

struct Vec3 {
    float x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vec3 operator-(const Vec3& v) const {
        return {x - v.x, y - v.y, z - v.z};
    }
    
    Vec3 operator+(const Vec3& v) const {
        return {x + v.x, y + v.y, z + v.z};
    }
    
    Vec3 operator*(float scalar) const {
        return {x * scalar, y * scalar, z * scalar};
    }
    
    Vec3 cross(const Vec3& v) const {
        return {
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        };
    }
    
    float dot(const Vec3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    Vec3 normalize() const {
        float len = std::sqrt(x * x + y * y + z * z);
        return {x / len, y / len, z / len};
    }
    
};

struct Ray {
    Vec3 origin, direction;
};


struct Intersection {
    bool hit;
    float distance;
    Vec3 point;
    int triangleIndex;
};

typedef struct Triangle {
    Vec3 a;
    Vec3 b;
    Vec3 c;
} Triangle;