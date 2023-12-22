#include "spine_texture_loader.h"

#include <string>

#include <spine/Atlas.h>

#include "../../../application/application.h"

namespace snowpulse {
std::shared_ptr<SpineTextureLoader> SpineTextureLoader::Create() {
    auto spineTextureLoader = std::shared_ptr<SpineTextureLoader>(new SpineTextureLoader());
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
    graphics_->LoadTexture(path.buffer(), PathType::kRaw);
    auto size = graphics_->GetTextureSize(path.buffer());
    auto ptr = new std::string(path.buffer());
    page.texture = (void*)ptr;
    page.width = size.x;
    page.height = size.y;
}

void SpineTextureLoader::unload(void *texture) {
    auto path = (std::string*)texture;
    graphics_->UnloadTexture(path->c_str());
    SPSAFE_DELETE(path);
}
}   // namespace snowpulse
