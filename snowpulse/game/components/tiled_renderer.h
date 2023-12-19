#ifndef SNOWPULSE_GAME_COMPONENTS_TILEDRENDERER_H_
#define SNOWPULSE_GAME_COMPONENTS_TILEDRENDERER_H_

#include "sprite_renderer.h"

#include <memory>
#include <string>
#include <vector>

#include "../../common/path_type.h"
#include "../plugins/tiled/tiled_map.h"

namespace snowpulse {
class SNOWPULSEAPI TiledRenderer : public SpriteRenderer {
    public:
        static std::shared_ptr<TiledRenderer> Create(std::string tiledFilename, PathType pathType = PathType::kAssets, TextureFiltering filtering = TextureFiltering::kBilinear);

        virtual ~TiledRenderer();
        // From Updatable
        void Update(float deltaTime) override;
        // From Drawable
        void Draw(Graphics* graphics, Matrix4x4 worldMatrix) override;

    protected:
        TiledRenderer();

        void LoadGraphics(Graphics* graphics) override;

        std::shared_ptr<TiledMap> tiledMap_;
};
}   // namespace snowpulse

#endif
