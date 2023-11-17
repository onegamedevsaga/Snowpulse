#include "spine_texture_loader.h"

#include "../../../application/application.h"

namespace snowpulse {
SpineTextureLoader::SpineTextureLoader() : graphics_(SPNULL){
}

SpineTextureLoader::~SpineTextureLoader() {
}

void SpineTextureLoader::load(spine::AtlasPage &page, const spine::String &path) {
    if (!graphics_) {
        graphics_ = Application::GetInstance()->GetGraphics();
    }
    graphics_->LoadTexture(path.buffer());
    //page.texture
}

void SpineTextureLoader::unload(void *texture) {
    
}
}   // namespace snowpulse
