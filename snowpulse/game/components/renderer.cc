#include "renderer.h"

#include "../../common/singleton.h"
#include "../../application/application.h"
#include "../../application/atlas_manager.h"
#include "../../application/directory.h"

namespace snowpulse {
Renderer::Renderer(std::string componentName) : Component(componentName), size_(100.0f, 100.0f), sortOrder_(0), blendMode_(BlendMode::kNormal), isPremultiplied_(false)  {
}

Renderer::~Renderer() {
}

void Renderer::LoadGraphics(Graphics* graphics) {
    graphics_ = graphics;
}
}   // namespace snowpulse
