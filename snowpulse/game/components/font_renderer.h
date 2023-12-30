#ifndef SNOWPULSE_GAME_COMPONENTS_FONTRENDERER_H_
#define SNOWPULSE_GAME_COMPONENTS_FONTRENDERER_H_

#include "sprite_renderer.h"

#include <memory>
#include <string>
#include <vector>

#include "../../common/path_type.h"
#include "../../common/text_alignment.h"
#include "../../application/font_manager.h"

namespace snowpulse {
class SNOWPULSEAPI FontRenderer : public SpriteRenderer {
    public:
        static std::shared_ptr<FontRenderer> Create(std::string fontFilename, int fontSizeInPixels, PathType pathType = PathType::kAssets, TextureFiltering filtering = TextureFiltering::kBilinear);

        virtual ~FontRenderer();
        // From Updatable
        void Update(float deltaTime) override;
        // From Drawable
        void Draw(Graphics* graphics, Matrix4x4 worldMatrix) override;

        void SetText(std::string text);
        void SetAlignment(TextAlignment alignment);
        void SetHorizontalSpacing(float spacing) { horizontalSpacing_ = spacing; }

    protected:
        class CharacterData {
            public:
                bool isNewLine;
                std::string textureFilename;
                Vector2 size;
                Vector2 offset;
                Vector2 uvLowerLeft;
                Vector2 uvUpperRight;
        };

        FontRenderer();

        void LoadGraphics(Graphics* graphics) override;

        FontManager* fontManager_;
        int fontSizeInPixels_;
        float horizontalSpacing_;
        std::string text_;
        std::vector<CharacterData> characterData_;
        int lineCount_;
        bool hasAligned_;
        TextAlignment alignment_;
        std::vector<float> positionOffsets_;
};
}   // namespace snowpulse

#endif
