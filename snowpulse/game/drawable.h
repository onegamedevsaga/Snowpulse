#ifndef SNOWPULSE_GAME_DRAWABLE_H_
#define SNOWPULSE_GAME_DRAWABLE_H_

#include "../defines.h"

#include "../application/graphics.h"

namespace snowpulse {
class SNOWPULSEAPI Drawable {
    public:
        virtual ~Drawable() = default;
        virtual void Draw(Graphics* graphics, Matrix4x4 worldMatrix) = 0;
};
}   // namespace snowpulse

#endif
