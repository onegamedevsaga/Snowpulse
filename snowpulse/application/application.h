#ifndef SNOWPULSE_APPLICATION_APPLICATION_H_
#define SNOWPULSE_APPLICATION_APPLICATION_H_

#include <string>
#include <memory>

#include "timer.h"
#include "platform.h"
#include "../defines.h"
#include "../common/vector2int.h"
#include "../game/graphics.h"
#include "../game/game.h"
#include "../game/node_starter.h"
#include "../game/action_manager.h"

namespace snowpulse {
class SNOWPULSEAPI Application {
    public:
        static Application* GetInstance();

        virtual ~Application();
        virtual bool Initialize(const Vector2Int& resolution, const Vector2Int& screenSize);
        virtual void Close() = 0;
        virtual void Shutdown() = 0;
        virtual Graphics* GetGraphics() const = 0;

        void SetGame(Game* game);

        Vector2Int GetResolutionSize() { return resolutionSize_; }
        Vector2Int GetScreenSize() { return screenSize_; }
        NodeStarter& GetNodeStarter() { return nodeStarter_; }
        Platform GetPlatform() const { return platform_; }
        std::string GetPlatformString() const { return platformString_; }
        Game* GetGame() const { return game_; }
        ActionManager* GetActionManager() const { return actionManager_.get(); }

    protected:
        Application();

        Platform platform_;
        Vector2Int screenSize_;
        Vector2Int resolutionSize_;
        NodeStarter nodeStarter_;
        Timer appTimer_;
        Timer frameTimer_;
        Game* game_;
        std::string platformString_;
        std::shared_ptr<ActionManager> actionManager_;

        float GetDeltaTime(Timer& timer);

};
}   // namespace snowpulse

#endif
