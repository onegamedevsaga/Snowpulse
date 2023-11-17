#ifndef SNOWPULSE_GAME_PLUGINS_SPINE_SPINESKELETONANIMATION_H_
#define SNOWPULSE_GAME_PLUGINS_SPINE_SPINESKELETONANIMATION_H_

#include "../../../defines.h"

#include <memory>
#include <string>

#include <spine/Atlas.h>
#include <spine/SkeletonData.h>
#include <spine/AnimationStateData.h>

#include "spine_texture_loader.h"
#include "../../updatable.h"
#include "../../drawable.h"

namespace snowpulse {
class SNOWPULSEAPI SpineSkeletonAnimation : public Updatable, public Drawable {
    public:
        static std::shared_ptr<SpineSkeletonAnimation> Create(std::string jsonFilename, std::string atlasFilename);

        virtual ~SpineSkeletonAnimation();
        // From Updatable
        void Update(float deltaTime);
        // From Drawable
        void Draw(Graphics* graphics, Matrix4x4 worldMatrix);

    protected:
        SpineSkeletonAnimation();

        std::shared_ptr<SpineTextureLoader> textureLoader_;
        std::shared_ptr<spine::Atlas> atlas_;
        std::shared_ptr<spine::SkeletonData> skeletonData_;
        std::shared_ptr<spine::AnimationStateData> animationStateData_;
};
}   // namespace snowpulse
#endif
