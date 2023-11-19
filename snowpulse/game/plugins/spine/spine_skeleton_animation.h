#ifndef SNOWPULSE_GAME_PLUGINS_SPINE_SPINESKELETONANIMATION_H_
#define SNOWPULSE_GAME_PLUGINS_SPINE_SPINESKELETONANIMATION_H_

#include "../../../defines.h"

#include <memory>
#include <string>

#include <spine/Atlas.h>
#include <spine/Skeleton.h>
#include <spine/SkeletonData.h>
#include <spine/AnimationState.h>
#include <spine/AnimationStateData.h>

#include "spine_texture_loader.h"
#include "../../drawable.h"

namespace snowpulse {
class SNOWPULSEAPI SpineSkeletonAnimation : public Updatable{
    public:
        static std::shared_ptr<SpineSkeletonAnimation> Create(std::string jsonFilename, std::string atlasFilename);

        virtual ~SpineSkeletonAnimation();
        // From Updatable
        void Update(float deltaTime);
        // From Drawable
        void Draw(Graphics* graphics, Matrix4x4 worldMatrix, int sortOrder);

    protected:
        SpineSkeletonAnimation();

        std::shared_ptr<SpineTextureLoader> textureLoader_;
        std::shared_ptr<spine::Atlas> atlas_;
        std::shared_ptr<spine::Skeleton> skeleton_;
        std::shared_ptr<spine::SkeletonData> skeletonData_;
        std::shared_ptr<spine::AnimationState> animationState_;
        std::shared_ptr<spine::AnimationStateData> animationStateData_;

        std::string atlasPath_;
};
}   // namespace snowpulse
#endif
