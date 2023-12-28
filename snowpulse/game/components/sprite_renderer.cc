#include "sprite_renderer.h"

#include "../../common/singleton.h"
#include "../../application/application.h"
#include "../../application/atlas_manager.h"
#include "../../application/directory.h"

namespace snowpulse {
std::shared_ptr<SpriteRenderer> SpriteRenderer::Create(std::string filename, PathType pathType, TextureFiltering filtering) {
    auto filenameOnly = Directory::GetInstance()->GetFilenameFromPath(filename);
    auto atlasManager = Application::GetInstance()->GetAtlasManager();
    auto spr = atlasManager->GetSprite(filenameOnly);
    if (spr.IsValid()) {
        return Create(filenameOnly, spr.GetAtlasFilename(), pathType, filtering);
    }

    auto spriteRenderer = std::shared_ptr<SpriteRenderer>(new SpriteRenderer("sprite_renderer"));
    spriteRenderer->isFromAtlas_ = false;
    spriteRenderer->filename_ = filename;
    spriteRenderer->pathType_ = pathType;
    spriteRenderer->textureFiltering_ = filtering;
    spriteRenderer->LoadGraphics(Application::GetInstance()->GetGraphics());
    return spriteRenderer;
}

std::shared_ptr<SpriteRenderer> SpriteRenderer::Create(std::string filename, std::string atlasFilename, PathType pathType, TextureFiltering filtering) {
    auto spriteRenderer = std::shared_ptr<SpriteRenderer>(new SpriteRenderer("sprite_renderer"));
    spriteRenderer->isFromAtlas_ = true;
    spriteRenderer->filename_ = filename;
    spriteRenderer->atlasFilename_ = atlasFilename;
    spriteRenderer->pathType_ = pathType;
    spriteRenderer->textureFiltering_ = filtering;
    spriteRenderer->LoadGraphics(Application::GetInstance()->GetGraphics());
    return spriteRenderer;
}

SpriteRenderer::SpriteRenderer(std::string componentName) : Renderer(componentName) {
}

SpriteRenderer::~SpriteRenderer() {
}

void SpriteRenderer::LoadGraphics(Graphics* graphics) {
    Renderer::LoadGraphics(graphics);

    if (isFromAtlas_) {
        auto atlasManager = Application::GetInstance()->GetAtlasManager();
        atlasManager->Load(atlasFilename_, pathType_);
        atlasSprite_ = atlasManager->GetSprite(atlasFilename_, filename_);
        if (atlasSprite_.IsValid()) {
            SetSize(atlasSprite_.GetSize());
            auto textureFilename = atlasSprite_.GetTextureFilename();
            auto uv = atlasSprite_.GetUV();
            auto atlasSize = graphics_->GetTextureSize(textureFilename);
            float left = uv.x;
            float right = uv.x + size_.x / atlasSize.x;
            float top = uv.y;
            float bottom = uv.y + size_.y / atlasSize.y;
            uvLowerLeft_ = Vector2(left, bottom);
            uvUpperRight_ = Vector2(right, top);
        }
    }
    else {
        graphics_->LoadTexture(filename_, pathType_);
        SetSize(graphics_->GetTextureSize(filename_));
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

void SpriteRenderer::SetTextureFilename(std::string filename, PathType pathType) {
    isFromAtlas_ = false;
    filename_ = filename;
    pathType_ = pathType;

    auto filenameOnly = Directory::GetInstance()->GetFilenameFromPath(filename);
    auto atlasManager = Application::GetInstance()->GetAtlasManager();
    auto spr = atlasManager->GetSprite(filenameOnly);
    if (spr.IsValid()) {
        isFromAtlas_ = true;
        atlasFilename_ = spr.GetAtlasFilename();
    }

    LoadGraphics(Application::GetInstance()->GetGraphics());
}
}   // namespace snowpulse
