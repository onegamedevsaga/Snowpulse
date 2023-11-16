#ifndef SNOWPULSE_COMMON_VECTOR3_H_
#define SNOWPULSE_COMMON_VECTOR3_H_

#include "../defines.h"

#include <cmath>

namespace snowpulse {
class SNOWPULSEAPI Vector3 {
    public:
        Vector3(float x = 0.0, float y = 0.0, float z = 0.0) : x(x), y(y), z(z) { }

        Vector3 operator+(const Vector3& other) const {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        Vector3 operator-(const Vector3& other) const {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }

        Vector3 operator*(float scalar) const {
            return Vector3(x * scalar, y * scalar, z * scalar);
        }

        Vector3 operator*(const Vector3& other) const {
            return Vector3(x * other.x, y * other.y, z * other.z);
        }

        Vector3& operator=(const Vector3& other) {
            if (this == &other) return *this;
            x = other.x;
            y = other.y;
            z = other.z;
            return *this;
        }

        Vector3& operator+=(const Vector3& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        Vector3& operator-=(const Vector3& other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        Vector3& operator*=(const Vector3& other) {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            return *this;
        }

        Vector3& operator/=(const Vector3& other) {
            x /= other.x;
            y /= other.y;
            z /= other.z;
            return *this;
        }

        bool operator==(const Vector3& other) const {
            return (x == other.x && y == other.y && z == other.z);
        }

        bool operator!=(const Vector3& other) const {
            return !(*this == other);
        }

        float magnitude() const {
            return std::sqrt(x * x + y * y + z * z);
        }

        Vector3 normalized() const {
            float mag = magnitude();
            if (mag != 0.0) {
                return Vector3(x / mag, y / mag, z / mag);
            }
            else {
                return Vector3();
            }
        }

        Vector3 cross(const Vector3& other) const {
            return Vector3(
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x
            );
        }

        float dot(const Vector3& other) const {
            return x * other.x + y * other.y + z * other.z;
        }

        float x;
        float y;
        float z;
};
}   // namespace snowpulse
#endif
