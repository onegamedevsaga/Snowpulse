#ifndef SNOWPULSE_APPLICATION_CAMERA_H_
#define SNOWPULSE_APPLICATION_CAMERA_H_

#include "../defines.h"

#include <memory>

#include "../common/vector2.h"
#include "../common/vector2int.h"
#include "../common/vector3.h"
#include "../common/matrix4x4.h"

namespace snowpulse {

// Forward declaration
class Application;

class SNOWPULSEAPI Camera {
    public:
        static std::shared_ptr<Camera> Create(const Vector2Int& resolution);

        virtual ~Camera();

        Matrix4x4 GetMatrix() const;
        Vector3 GetRawPosition() const;
        Vector3 GetPosition() const { return position_; }
        void SetPosition(Vector2 position);

    private:
        Camera();

        Application* application_;
        Vector3 position_;
};
}   // namespace snowpulse
#endif
