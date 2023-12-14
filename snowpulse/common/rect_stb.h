#ifndef SNOWPULSE_COMMON_RECTSTB_H_
#define SNOWPULSE_COMMON_RECTSTB_H_

#include "../defines.h"
#include "rect.h"

namespace snowpulse {
class SNOWPULSEAPI RectSTB : public Rect {
    public:
        RectSTB() : Rect(), id_(-1), isPacked_(false) { }
        RectSTB(int rectId, Vector2 position, Vector2 size, bool isPacked) : Rect(position, size), id_(rectId), isPacked_(isPacked) { }
        RectSTB(int rectId, float x, float y, float width, float height, bool isPacked) : Rect(x, y, width, height), id_(rectId), isPacked_(isPacked) { }

        void SetId(int rectId) { id_ = rectId; }
        void SetIsPacked(bool isPacked) { isPacked_ = isPacked; }

        int GetId() const { return id_; }
        bool IsPacked() const { return isPacked_; }

    private:
        int id_;
        bool isPacked_;
};
}   // namespace snowpulse
#endif
