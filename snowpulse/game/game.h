#ifndef SNOWPULSE_GAME_GAME_H_
#define SNOWPULSE_GAME_GAME_H_

#include "../defines.h"

#include <memory>

#include "../game/node.h"
#include "../game/updatable.h"
#include "../game/drawable.h"
#include "../game/scene.h"

namespace snowpulse {

// Forward declaration
class Application;

class SNOWPULSEAPI Game : public Node {
    public:
        virtual ~Game();
        virtual void Shutdown();
        virtual void Initialize(Application* application);

        Application* GetApplication() { return application_; }
        Scene* GetScene() { return scene_.get(); }

        void Start();
        void SetScene(std::shared_ptr<Scene> scene);
        void UpdateScene(float deltaTime);
        void DrawScene(Graphics* graphics);

    protected:
        Game();

        Application* application_;
        std::shared_ptr<Scene> scene_;
};
}   // namespace snowpulse
#endif
