#ifndef SNOWPULSE_GAME_COMPONENTS_RECTRENDERER_H_
#define SNOWPULSE_GAME_COMPONENTS_RECTRENDERER_H_

#include "../../defines.h"
#include "renderer.h"

#include <memory>
#include <string>

#include "../drawable.h"
#include "../../common/vector2.h"
#include "../../common/blend_mode.h"
#include "../../common/color.h"

namespace snowpulse {
class SNOWPULSEAPI RectRenderer : public Renderer {
    public:
        static std::shared_ptr<RectRenderer> Create(Vector2 size);

        virtual ~RectRenderer();
        // From Updatable
        void Update(float deltaTime) override;
        // From Drawable
        void Draw(Graphics* graphics, Matrix4x4 worldMatrix) override;

    protected:
        RectRenderer(std::string componentName);

        virtual void LoadGraphics(Graphics* graphics) override;
};
}   // namespace snowpulse

#endif
