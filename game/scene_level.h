#ifndef GAME_SCENELEVEL_H_
#define GAME_SCENELEVEL_H_

#include "defines.h"
#include <snowpulse.h>

#include <memory>

namespace game {
class GAMEAPI SceneLevel : public snowpulse::Scene {
    public:
        static std::shared_ptr<SceneLevel> Create(snowpulse::Application* application);

        virtual ~SceneLevel();
        void Initialize(snowpulse::Application* application);
        void Shutdown();
        void Start();
        void Update(float deltaTime);

    private:
        SceneLevel();

    std::shared_ptr<snowpulse::GameObject> go1_, go2_, go3_;
};
}

#endif
