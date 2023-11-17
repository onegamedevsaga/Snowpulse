#include "spine_skeleton_animation.h"

#include <iostream>

#include <spine/Extension.h>
#include <spine/SkeletonJson.h>

#include "spine_texture_loader.h"

spine::SpineExtension *spine::getDefaultExtension() {
    return new spine::DefaultSpineExtension();
}

namespace snowpulse {
std::shared_ptr<SpineSkeletonAnimation> SpineSkeletonAnimation::Create(std::string jsonFilename, std::string atlasFilename) {
    auto spineSkeletonAnimation = std::shared_ptr<SpineSkeletonAnimation>(new SpineSkeletonAnimation());
    spineSkeletonAnimation->textureLoader_ = SpineTextureLoader::Create();
    spineSkeletonAnimation->atlas_ = std::make_shared<spine::Atlas>(atlasFilename.c_str(), spineSkeletonAnimation->textureLoader_.get());
    if (spineSkeletonAnimation->atlas_->getPages().size() == 0) {
#ifdef SPDEBUG
    std::cerr << "Error: Failed to load spine atlas (" << atlasFilename << ")." << std::endl;
#endif
        return SPNULL;
    }

    spine::SkeletonJson json(spineSkeletonAnimation->atlas_.get());
    spineSkeletonAnimation->skeletonData_ = std::shared_ptr<spine::SkeletonData>(json.readSkeletonDataFile(jsonFilename.c_str()));
    if (!spineSkeletonAnimation->skeletonData_) {
#ifdef SPDEBUG
    std::cerr << "Error: Failed to load spine json (" << jsonFilename << ")." << std::endl;
#endif
        return SPNULL;
    }

    spineSkeletonAnimation->animationStateData_ = std::make_shared<spine::AnimationStateData>(spineSkeletonAnimation->skeletonData_.get());
    //spineSkeletonAnimation->animationStateData_->
    //spineSkeletonAnimation->animationStateData_->setDefaultMix(0.5f);
    //spineSkeletonAnimation->animationStateData_->setMix("walk", "run", 0.2f);
    //spineSkeletonAnimation->animationStateData_->setMix("walk", "shot", 0.1f);
    
    spineSkeletonAnimation->skeleton_ = std::make_shared<spine::Skeleton>(spineSkeletonAnimation->skeletonData_.get());
    spineSkeletonAnimation->animationState_ = std::make_shared<spine::AnimationState>(spineSkeletonAnimation->animationStateData_.get());
    spineSkeletonAnimation->animationState_->setAnimation(0, "idle", true);
    return spineSkeletonAnimation;
}

SpineSkeletonAnimation::SpineSkeletonAnimation() {
}

SpineSkeletonAnimation::~SpineSkeletonAnimation() {
}

// From Updatable
void SpineSkeletonAnimation::Update(float deltaTime) {
    animationState_->update(deltaTime);
    animationState_->apply(*skeleton_);
    skeleton_->updateWorldTransform();
}

// From Drawable
void SpineSkeletonAnimation::Draw(Graphics* graphics, Matrix4x4 worldMatrix) {
    //graphics->DrawSpineSkeleton(skeleton_.get(), worldMatrix, color_, sortOrder_);
}
}   // namespace snowpulse
