#ifndef SNOWPULSE_GAME_COMPONENTS_SPRITERENDERER_H_
#define SNOWPULSE_GAME_COMPONENTS_SPRITERENDERER_H_

#include "../../defines.h"
#include "../component.h"

#include <memory>
#include <string>

#include "../drawable.h"
#include "../../common/vector2.h"
#include "../../common/blend_mode.h"
#include "../../common/color.h"

namespace snowpulse {
class SNOWPULSEAPI SpriteRenderer : public Component, public Drawable {
    public:
        static std::shared_ptr<SpriteRenderer> Create(std::string filename);

        virtual ~SpriteRenderer();
        // From Updatable
        void Update(float deltaTime);
        // From Drawable
        void Draw(Graphics* graphics, Matrix4x4 worldMatrix);

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
        SpriteRenderer();
        SpriteRenderer(std::string componentName);

        virtual void LoadGraphics(Graphics* graphics);

        int sortOrder_;
        Vector2 size_;
        Color color_;
        BlendMode blendMode_;
        bool isPremultiplied_;
        std::string filename_;
};
}   // namespace snowpulse

#endif
