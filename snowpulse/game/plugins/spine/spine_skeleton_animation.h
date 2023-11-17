#ifndef SNOWPULSE_GAME_PLUGINS_SPINE_SPINESKELETONANIMATION_H_
#define SNOWPULSE_GAME_PLUGINS_SPINE_SPINESKELETONANIMATION_H_

#include "../../../defines.h"

#include <memory>
#include <string>

#include "../../updatable.h"
#include "../../drawable.h"

namespace snowpulse {
class SNOWPULSEAPI SpineSkeletonAnimation : public Updatable, public Drawable {
    public:
        static std::shared_ptr<SpineSkeletonAnimation> Create(std::string jsonFilename);

        virtual ~SpineSkeletonAnimation();
        // From Updatable
        void Update(float deltaTime);
        // From Drawable
        void Draw(Graphics* graphics, Matrix4x4 worldMatrix);

    protected:
        SpineSkeletonAnimation();

        std::string filename_;
};
}   // namespace snowpulse
#endif
