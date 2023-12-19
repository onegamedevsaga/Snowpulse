#include "tiled_renderer.h"

#include "../../application/application.h"

namespace snowpulse {
std::shared_ptr<TiledRenderer> TiledRenderer::Create(std::string tiledFilename, PathType pathType, TextureFiltering filtering) {
    auto tiledRenderer = std::shared_ptr<TiledRenderer>(new TiledRenderer());
    tiledRenderer->filename_ = tiledFilename;
    tiledRenderer->pathType_ = pathType;
    tiledRenderer->textureFiltering_ = filtering;
    tiledRenderer->LoadGraphics(Application::GetInstance()->GetGraphics());
    return tiledRenderer;
}

TiledRenderer::TiledRenderer() : SpriteRenderer("tiled_renderer") {
}

TiledRenderer::~TiledRenderer() {
}

void TiledRenderer::LoadGraphics(Graphics* graphics) {
    graphics_ = graphics;
    tiledMap_ = TiledMap::Create(filename_, pathType_, textureFiltering_);
}

// From Updatable
void TiledRenderer::Update(float deltaTime) {
    Component::Update(deltaTime);
}

// From Drawable
void TiledRenderer::Draw(Graphics* graphics, Matrix4x4 worldMatrix) {
    
}
}   // namespace snowpulse
