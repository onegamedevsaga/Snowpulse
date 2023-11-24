#include "spine_texture_loader.h"

#include <string>

#include <spine/Atlas.h>

#include "../../../application/application.h"

namespace snowpulse {
std::shared_ptr<SpineTextureLoader> SpineTextureLoader::Create(TextureFiltering filtering) {
    auto spineTextureLoader = std::shared_ptr<SpineTextureLoader>(new SpineTextureLoader());
    spineTextureLoader->textureFiltering_ = filtering;
    return spineTextureLoader;
}

SpineTextureLoader::SpineTextureLoader() : graphics_(SPNULL){
}

SpineTextureLoader::~SpineTextureLoader() {
}

void SpineTextureLoader::load(spine::AtlasPage &page, const spine::String &path) {
    if (!graphics_) {
        graphics_ = Application::GetInstance()->GetGraphics();
    }
    graphics_->LoadTexture(path.buffer(), textureFiltering_, true);
    auto size = graphics_->GetTextureSize(path.buffer(), textureFiltering_);
    auto ptr = new std::string(path.buffer());
    page.texture = (void*)ptr;
    page.width = size.x;
    page.height = size.y;
}

void SpineTextureLoader::unload(void *texture) {
    auto path = (std::string*)texture;
    graphics_->UnloadTexture(path->c_str(), textureFiltering_);
    SPSAFE_DELETE(path);
}
}   // namespace snowpulse
