#ifndef SNOWPULSE_GAME_COMPONENTS_SPINERENDERER_H_
#define SNOWPULSE_GAME_COMPONENTS_SPINERENDERER_H_

#include "sprite_renderer.h"

#include <memory>

#include "../plugins/spine/spine_skeleton_animation.h"

namespace snowpulse {
class SNOWPULSEAPI SpineRenderer : public SpriteRenderer {
    public:
        static std::shared_ptr<SpineRenderer> Create(std::string jsonFilename, std::string atlasFilename);

        virtual ~SpineRenderer();
        // From Updatable
        void Update(float deltaTime);
        // From Drawable
        void Draw(Graphics* graphics, Matrix4x4 worldMatrix);

        int GetSortOrder() { return sortOrder_; }
        Vector2 GetSize() { return size_; }
        Color GetColor() { return color_; }

        void SetSortOrder(int sortOrder) { sortOrder_ = sortOrder; }
        void SetSize(Vector2 size) { size_ = size; }
        void SetColor(Color color) { color_ = color; }

    protected:
        SpineRenderer();

        void LoadGraphics(Graphics* graphics);

        std::shared_ptr<SpineSkeletonAnimation> skeletonAnimation_;
};
}   // namespace snowpulse

#endif