#include "spine_skeleton_animation.h"

#include <spine/Extension.h>

spine::SpineExtension *spine::getDefaultExtension() {
    return new spine::DefaultSpineExtension();
}

namespace snowpulse {
std::shared_ptr<SpineSkeletonAnimation> SpineSkeletonAnimation::Create(std::string jsonFilename) {
    auto skeletonAnimation = std::shared_ptr<SpineSkeletonAnimation>(new SpineSkeletonAnimation());
    return skeletonAnimation;
}

SpineSkeletonAnimation::SpineSkeletonAnimation() {
}

SpineSkeletonAnimation::~SpineSkeletonAnimation() {
}

// From Updatable
void SpineSkeletonAnimation::Update(float deltaTime) {
}

// From Drawable
void SpineSkeletonAnimation::Draw(Graphics* graphics, Matrix4x4 worldMatrix) {
}
}   // namespace snowpulse
