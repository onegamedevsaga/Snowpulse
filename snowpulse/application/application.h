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
        virtual void SetScreenSize(const Vector2Int& screenSize);
        virtual void Close() = 0;
        virtual void Shutdown() = 0;
        virtual void SetScreenScaleFactor(float scaleFactor) { scaleFactor_ = scaleFactor; }
        virtual Graphics* GetGraphics() const = 0;

        void SetGame(Game* game);
        void SetAssetFolderPath(std::string path) { assetFolderPath_ = path; }

        float GetScaleFactor() const { return scaleFactor_; }
        Vector2Int GetResolutionSize() const { return resolutionSize_; }
        Vector2Int GetScreenSize() const { return screenSize_; }
        NodeStarter& GetNodeStarter() { return nodeStarter_; }
        Platform GetPlatform() const { return platform_; }
        std::string GetPlatformString() const { return platformString_; }
        Game* GetGame() const { return game_; }
        ActionManager* GetActionManager() const { return actionManager_.get(); }
        std::string GetAssetFolderPath() const { return assetFolderPath_; }

    protected:
        Application();

        Platform platform_;
        float scaleFactor_;
        Vector2Int resolutionSize_;
        Vector2Int targetResolutionSize_;
        Vector2Int screenSize_;
        NodeStarter nodeStarter_;
        Timer appTimer_;
        Timer frameTimer_;
        Game* game_;
        std::string platformString_;
        std::shared_ptr<ActionManager> actionManager_;
        std::string assetFolderPath_;

        float GetDeltaTime(Timer& timer);

};
}   // namespace snowpulse

#endif
