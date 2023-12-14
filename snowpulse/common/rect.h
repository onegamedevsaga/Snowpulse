#ifndef SNOWPULSE_COMMON_RECT_H_
#define SNOWPULSE_COMMON_RECT_H_

#include "../defines.h"

#include "vector2.h"

namespace snowpulse {
class SNOWPULSEAPI Rect {
    public:
        Rect() : position_(), size_() { }
        Rect(Vector2 position, Vector2 size) : position_(position), size_(size) { }
        Rect(float x, float y, float width, float height) : position_(x, y), size_(width, height) { }

        void SetPosition(Vector2 position) { position_ = position; }
        void SetSize(Vector2 size) { size_ = size; }
        void SetPosition(float x, float y) { position_.x = x; position_.y = y; }
        void SetSize(float x, float y) { size_.x = x; size_.y = y; }

        Vector2 GetPosition() const { return position_; }
        Vector2 GetSize() const { return size_; }
        int GetX() const { return position_.x; }
        int GetY() const { return position_.y; }
        int GetWidth() const { return size_.x; }
        int GetHeight() const { return size_.y; }

    protected:
        Vector2 position_;
        Vector2 size_;
};
}   // namespace snowpulse
#endif
