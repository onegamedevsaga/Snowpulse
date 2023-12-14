#ifndef SNOWPULSE_COMMON_MATRIX4X4_H_
#define SNOWPULSE_COMMON_MATRIX4X4_H_

#include "../defines.h"

#include "vector3.h"

namespace snowpulse {
class Matrix4x4 {
    public:

        static Matrix4x4 Rotate(const Vector3& eulerAngles) {
            Matrix4x4 result;
            float multiplier = 3.14159265358979323846f / 180.0f;
            float radiansY = eulerAngles.y * multiplier;
            float radiansX = eulerAngles.x * multiplier;
            float radiansZ = eulerAngles.z * multiplier;

            float cosX = cosf(radiansX);
            float sinX = sinf(radiansX);
            float cosY = cosf(radiansY);
            float sinY = sinf(radiansY);
            float cosZ = cosf(radiansZ);
            float sinZ = sinf(radiansZ);

            result.data[0][0] = cosY * cosZ;
            result.data[0][1] = -cosY * sinZ;
            result.data[0][2] = sinY;
            result.data[0][3] = 0.0f;

            result.data[1][0] = sinX * sinY * cosZ + cosX * sinZ;
            result.data[1][1] = cosX * cosZ - sinX * sinY * sinZ;
            result.data[1][2] = -sinX * cosY;
            result.data[1][3] = 0.0f;

            result.data[2][0] = -cosX * sinY * cosZ + sinX * sinZ;
            result.data[2][1] = sinY * sinZ + cosX * sinX * cosZ;
            result.data[2][2] = cosX * cosY;
            result.data[2][3] = 0.0f;

            result.data[3][0] = 0.0f;
            result.data[3][1] = 0.0f;
            result.data[3][2] = 0.0f;
            result.data[3][3] = 1.0f;

            return result;
        }

        static Matrix4x4 Scale(const Vector3& scale) {
            Matrix4x4 result;
            result.data[0][0] = scale.x;
            result.data[1][1] = scale.y;
            result.data[2][2] = scale.z;
            return result;
        }

        static Matrix4x4 Translate(const Vector3& position) {
            Matrix4x4 result;
            result.data[3][0] = position.x;
            result.data[3][1] = position.y;
            result.data[3][2] = position.z;
            return result;
        }

        static Matrix4x4 LookAtRH(const Vector3& eye, const Vector3& center, const Vector3& up) {
            Matrix4x4 result;

            Vector3 forward = (center - eye).normalized();
            Vector3 right = up.cross(forward).normalized();
            Vector3 realUp = forward.cross(right);

            result.data[0][0] = right.x;
            result.data[1][0] = right.y;
            result.data[2][0] = right.z;
            result.data[0][1] = realUp.x;
            result.data[1][1] = realUp.y;
            result.data[2][1] = realUp.z;
            result.data[0][2] = -forward.x;
            result.data[1][2] = -forward.y;
            result.data[2][2] = -forward.z;
            result.data[3][0] = -right.dot(eye);
            result.data[3][1] = -realUp.dot(eye);
            result.data[3][2] = forward.dot(eye);

            return result;
        }

        Matrix4x4() {
            SetIdentity();
        }

        void SetIdentity() {
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    data[i][j] = (i == j) ? 1.0f : 0.0f;
                }
            }
        }

        void Copy(const Matrix4x4& other) {
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    data[i][j] = other.data[i][j];
                }
            }
        }

        Matrix4x4 operator*(const Matrix4x4& other) const {
            Matrix4x4 result;
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    result.data[i][j] = 0.0f;
                    for (int k = 0; k < 4; ++k) {
                        result.data[i][j] += data[i][k] * other.data[k][j];
                    }
                }
            }
            return result;
        }

        Vector3 operator*(const Vector3& v) const {
            float result[3];
            for (int i = 0; i < 3; ++i) {
                result[i] = data[0][i] * v.x + data[1][i] * v.y + data[2][i] * v.z + data[3][i];
            }
            return Vector3(result[0], result[1], result[2]);
        }

        void AddTranslate(const Vector3& position) {
            data[3][0] += position.x;
            data[3][1] += position.y;
            data[3][2] += position.z;
        }

        void AddScale(const Vector3& scale) {
            data[0][0] += scale.x;
            data[1][1] += scale.y;
            data[2][2] += scale.z;
        }

        float data[4][4];
};
}   // namespace snowpulse
#endif
