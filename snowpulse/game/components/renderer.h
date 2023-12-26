#ifndef SNOWPULSE_GAME_COMPONENTS_RENDERER_H_
#define SNOWPULSE_GAME_COMPONENTS_RENDERER_H_

#include "../../defines.h"
#include "../component.h"

#include <memory>
#include <string>

#include "../drawable.h"
#include "../../common/vector2.h"
#include "../../common/blend_mode.h"
#include "../../common/color.h"
#include "../../common/texture_filtering.h"
#include "../../common/atlas_sprite.h"

namespace snowpulse {
class SNOWPULSEAPI Renderer : public Component, public Drawable {
    public:
        virtual ~Renderer();

        int GetSortOrder() const { return sortOrder_; }
        Vector2 GetSize() const { return size_; }
        BlendMode GetBlendMode() const { return blendMode_; }
        bool IsPremultiplied() const { return isPremultiplied_; }
        Color GetColor() const { return color_; }

        void SetSortOrder(int sortOrder) { sortOrder_ = sortOrder; }
        void SetSize(Vector2 size) { size_ = size; }
        void SetBlendMode(BlendMode blendMode) { blendMode_ = blendMode; }
        void SetIsPremultiplied(bool value) { isPremultiplied_ = value; }
        void SetColor(Color color) { color_ = color; }

    protected:
        Renderer(std::string componentName);

        virtual void LoadGraphics(Graphics* graphics);

        Graphics* graphics_;
        Vector2 size_;
        Color color_;
        BlendMode blendMode_;
        int sortOrder_;
        bool isPremultiplied_;
};
}   // namespace snowpulse

#endif
