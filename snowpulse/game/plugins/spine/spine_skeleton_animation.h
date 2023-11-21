#ifndef SNOWPULSE_GAME_PLUGINS_SPINE_SPINESKELETONANIMATION_H_
#define SNOWPULSE_GAME_PLUGINS_SPINE_SPINESKELETONANIMATION_H_

#include "../../../defines.h"

#include <memory>
#include <string>
#include <map>

#include <spine/Atlas.h>
#include <spine/Skeleton.h>
#include <spine/SkeletonData.h>
#include <spine/Slot.h>
#include <spine/BoneData.h>
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
        void Draw(Graphics* graphics, Matrix4x4 worldMatrix, int sortOrder, bool isPremultiplied);

        void SetSkin(std::string name);
        void PlayAnimation(std::string name, bool looping = false);

    protected:
        SpineSkeletonAnimation();
        std::vector<spine::Slot*> GetSlotParents(spine::Slot* slot);
        std::vector<spine::Slot*> GetSlotsWithBoneDatas(spine::Vector<spine::BoneData*> bones);
        bool IsSlotOnBones(spine::Slot* slot, spine::Vector<spine::BoneData*>& bones);

        std::shared_ptr<SpineTextureLoader> textureLoader_;
        std::shared_ptr<spine::Atlas> atlas_;
        std::shared_ptr<spine::Skeleton> skeleton_;
        std::shared_ptr<spine::SkeletonData> skeletonData_;
        std::shared_ptr<spine::AnimationState> animationState_;
        std::shared_ptr<spine::AnimationStateData> animationStateData_;

        std::string jsonFilename_;
        std::string atlasPath_;
        std::map<spine::Slot*, bool> slotsToDraw_;
};
}   // namespace snowpulse
#endif
