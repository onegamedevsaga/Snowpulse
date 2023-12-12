#include "sprite_renderer.h"

#include "../../common/singleton.h"
#include "../../application/application.h"
#include "../../application/atlas.h"

namespace snowpulse {
std::shared_ptr<SpriteRenderer> SpriteRenderer::Create(std::string filename, TextureFiltering filtering) {
    auto spriteRenderer = std::shared_ptr<SpriteRenderer>(new SpriteRenderer());
    spriteRenderer->isFromAtlas_ = false;
    spriteRenderer->filename_ = filename;
    spriteRenderer->textureFiltering_ = filtering;
    spriteRenderer->LoadGraphics(Application::GetInstance()->GetGraphics());
    return spriteRenderer;
}

std::shared_ptr<SpriteRenderer> SpriteRenderer::Create(std::string filename, std::string atlasFilename, TextureFiltering filtering) {
    auto spriteRenderer = std::shared_ptr<SpriteRenderer>(new SpriteRenderer());
    spriteRenderer->isFromAtlas_ = true;
    spriteRenderer->filename_ = filename;
    spriteRenderer->atlasFilename_ = atlasFilename;
    spriteRenderer->textureFiltering_ = filtering;
    spriteRenderer->LoadGraphics(Application::GetInstance()->GetGraphics());
    return spriteRenderer;
}

SpriteRenderer::SpriteRenderer() : Component("sprite_renderer"), sortOrder_(0), blendMode_(BlendMode::kNormal), isPremultiplied_(false) {
}

SpriteRenderer::SpriteRenderer(std::string componentName) : Component(componentName), sortOrder_(0)  {
}

SpriteRenderer::~SpriteRenderer() {
}

void SpriteRenderer::LoadGraphics(Graphics* graphics) {
    if (isFromAtlas_) {
        auto atlas = Application::GetInstance()->GetAtlas();
        atlas->Load(atlasFilename_);
        atlasSprite_ = atlas->GetSprite(atlasFilename_, filename_);
        if (atlasSprite_.GetFilename() == filename_) {
            SetSize(atlasSprite_.GetSize());
            auto textureFilename = atlasSprite_.GetTextureFilename();
            auto uv = atlasSprite_.GetUV();
            auto atlasSize = graphics->GetTextureSize(textureFilename);
            float left = uv.x;
            float right = uv.x + size_.x / atlasSize.x;
            float top = uv.y;
            float bottom = uv.y + size_.y / atlasSize.y;
            uvLowerLeft_ = Vector2(left, bottom);
            uvUpperRight_ = Vector2(right, top);
        }
        else {
            size_ = Vector2(100, 100);
        }
    }
    else {
        graphics->LoadTexture(filename_);
        SetSize(graphics->GetTextureSize(filename_));
    }
}

// From Updatable
void SpriteRenderer::Update(float deltaTime) {
    Component::Update(deltaTime);
}

// From Drawable
void SpriteRenderer::Draw(Graphics* graphics, Matrix4x4 worldMatrix) {
    if (isFromAtlas_) {
        graphics->DrawSprite(size_, atlasSprite_.GetTextureFilename(), worldMatrix, color_, sortOrder_, blendMode_, textureFiltering_, isPremultiplied_, uvLowerLeft_, uvUpperRight_);
    }
    else {
        graphics->DrawSprite(size_, filename_, worldMatrix, color_, sortOrder_, blendMode_, textureFiltering_, isPremultiplied_);
    }
}
}   // namespace snowpulse
