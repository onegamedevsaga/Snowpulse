#ifndef SNOWPULSE_APPLICATION_APPLICATION_H_
#define SNOWPULSE_APPLICATION_APPLICATION_H_

#include "../common/singleton.h"

#include <string>
#include <memory>

#include "timer.h"
#include "platform.h"
#include "camera.h"
#include "../defines.h"
#include "../game/graphics.h"
#include "../game/game.h"
#include "../game/node_starter.h"
#include "../game/action_manager.h"

namespace snowpulse {
class SNOWPULSEAPI Application : public Singleton<Application> {
    public:
        virtual ~Application();
        virtual void Run() = 0;
        virtual void Close() = 0;
        virtual void Shutdown() = 0;
        virtual Graphics* GetGraphics() const = 0;

        void SetGame(Game* game);

        NodeStarter& GetNodeStarter() { return nodeStarter_; }
        Platform GetPlatform() const { return platform_; }
        std::string GetPlatformString() const { return platformString_; }
        Game* GetGame() const { return game_; }
        Camera* GetCamera() const { return camera_.get(); }
        ActionManager* GetActionManager() const { return actionManager_.get(); }

    protected:
        Application();
        virtual bool Initialize();

        Platform platform_;
        NodeStarter nodeStarter_;
        Game* game_;
        std::string platformString_;
        std::shared_ptr<Camera> camera_;
        std::shared_ptr<ActionManager> actionManager_;

        float GetDeltaTime(Timer& timer);

};
}   // namespace snowpulse

#endif
