#include "font_renderer.h"

#include "../../common/path_type.h"
#include "../../common/atlas_sprite.h"
#include "../../application/application.h"
#include "../../application/atlas_manager.h"
#include "../../application/font_manager.h"


namespace snowpulse {
std::shared_ptr<FontRenderer> FontRenderer::Create(std::string fontFilename, int fontSizeInPixels, PathType pathType, TextureFiltering filtering) {
    auto fontRenderer = std::shared_ptr<FontRenderer>(new FontRenderer());
    fontRenderer->isFromAtlas_ = true;
    fontRenderer->filename_ = fontFilename;
    fontRenderer->fontSizeInPixels_ = fontSizeInPixels;
    fontRenderer->pathType_ = pathType;
    fontRenderer->textureFiltering_ = filtering;
    fontRenderer->LoadGraphics(Application::GetInstance()->GetGraphics());
    return fontRenderer;
}

FontRenderer::FontRenderer() : SpriteRenderer("font_renderer"), fontSizeInPixels_(10), horizontalSpacing_(100.0f), text_(""), lineCount_(1) {
}

FontRenderer::~FontRenderer() {
}

void FontRenderer::LoadGraphics(Graphics* graphics) {
    graphics_ = graphics;

    if (isFromAtlas_) {
        fontManager_ = Application::GetInstance()->GetFontManager();
        fontManager_->Load(filename_, fontSizeInPixels_, pathType_);
    }
}

// From Updatable
void FontRenderer::Update(float deltaTime) {
    Component::Update(deltaTime);
}

// From Drawable
void FontRenderer::Draw(Graphics* graphics, Matrix4x4 worldMatrix) {
    Matrix4x4 originalWorldMatrix(worldMatrix);
    Matrix4x4 matrixCopy(worldMatrix);
    float scaleX = worldMatrix.GetScaleX();
    float scaleY = worldMatrix.GetScaleY();
    auto horizontalSpacing = horizontalSpacing_ * fontSizeInPixels_ * 0.001f;
    int lineCount = 1;
    for (const auto& d : characterData_) {
        if (d.isNewLine) {
            worldMatrix.CopyTranslation(originalWorldMatrix);
            worldMatrix.AddTranslate(Vector3(0.0f, (float)lineCount * (float)-fontSizeInPixels_ * scaleY, 0.0f));
            lineCount ++;
            continue;
        }
        matrixCopy.CopyTranslation(worldMatrix);
        matrixCopy.AddTranslate(Vector3((d.offset.x + d.size.x * 0.5f) * scaleX, (-d.offset.y + d.size.y * -0.5f) * scaleY, 0.0f));
        graphics->DrawSprite(d.size, d.textureFilename, matrixCopy, color_, sortOrder_, blendMode_, textureFiltering_, isPremultiplied_, d.uvLowerLeft, d.uvUpperRight);
        worldMatrix.AddTranslate(Vector3((d.size.x + horizontalSpacing) * scaleX, 0.0f, 0.0f));
    }
}

void FontRenderer::SetText(std::string text) {
    text_ = text;
    size_ = Vector2();
    characterData_.clear();

    auto atlasManager = Application::GetInstance()->GetAtlasManager();
    for (const auto& c : text) {
        auto atlasSprite = atlasManager->GetSprite(fontManager_->GetCharacterSpriteName(filename_, fontSizeInPixels_, c));
        if (atlasSprite.IsValid()) {
            CharacterData d;
            d.isNewLine = false;
            d.textureFilename = atlasSprite.GetTextureFilename();
            d.size = atlasSprite.GetSize();
            d.size.x = d.size.x == 0.0f ? fontSizeInPixels_ * 0.3f : d.size.x;
            auto atlasSize = graphics_->GetTextureSize(d.textureFilename);
            auto uv = atlasSprite.GetUV();
            float left = uv.x;
            float right = uv.x + d.size.x / atlasSize.x;
            float top = uv.y;
            float bottom = uv.y + d.size.y / atlasSize.y;
            d.uvLowerLeft = Vector2(left, bottom);
            d.uvUpperRight = Vector2(right, top);
            d.offset = fontManager_->GetCharacterOffset(filename_, fontSizeInPixels_, c);
            characterData_.push_back(d);

            size_.x += d.size.x;
            size_.y = d.size.y > size_.y ? d.size.y : size_.y;
        }
        else if (c == '\n') {
            lineCount_ ++;
            CharacterData d;
            d.isNewLine = true;
            characterData_.push_back(d);
        }
    }
}
}   // namespace snowpulse
