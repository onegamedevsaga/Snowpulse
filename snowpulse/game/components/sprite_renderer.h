#ifndef SNOWPULSE_GAME_COMPONENTS_SPRITERENDERER_H_
#define SNOWPULSE_GAME_COMPONENTS_SPRITERENDERER_H_

#include "../../defines.h"
#include "renderer.h"

#include <memory>
#include <string>

#include "../drawable.h"
#include "../../common/vector2.h"
#include "../../common/blend_mode.h"
#include "../../common/color.h"
#include "../../common/texture_filtering.h"
#include "../../common/atlas_sprite.h"

namespace snowpulse {
class SNOWPULSEAPI SpriteRenderer : public Renderer {
    public:
        static std::shared_ptr<SpriteRenderer> Create(std::string filename, PathType pathType = PathType::kAssets, TextureFiltering filtering = TextureFiltering::kBilinear);
        static std::shared_ptr<SpriteRenderer> Create(std::string filename, std::string atlasFilename, PathType pathType = PathType::kAssets, TextureFiltering filtering = TextureFiltering::kBilinear);

        virtual ~SpriteRenderer();
        // From Updatable
        void Update(float deltaTime) override;
        // From Drawable
        void Draw(Graphics* graphics, Matrix4x4 worldMatrix) override;

    protected:
        SpriteRenderer(std::string componentName);

        virtual void LoadGraphics(Graphics* graphics) override;

        bool isFromAtlas_;
        AtlasSprite atlasSprite_;
        Vector2 uvLowerLeft_;
        Vector2 uvUpperRight_;
        PathType pathType_;
        TextureFiltering textureFiltering_;
        std::string filename_;
        std::string atlasFilename_;
};
}   // namespace snowpulse

#endif
