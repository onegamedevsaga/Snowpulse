#ifndef SNOWPULSE_GAME_SCENE_H_
#define SNOWPULSE_GAME_SCENE_H_

#include "../defines.h"

#include <vector>
#include <memory>

#include "node.h"
#include "updatable.h"
#include "drawable.h"
#include "game_object.h"
#include "../common/vector2int.h"

namespace snowpulse {

// Forward declaration
class Application;
class Game;

class SNOWPULSEAPI Scene : public Node, public Updatable, public Drawable {
    public:
        virtual ~Scene();
        virtual void Initialize(Application* application);
        virtual void Shutdown();

        // From Node
        virtual void Start();
        bool AddChild(std::shared_ptr<Node> node);
        bool RemoveChild(std::shared_ptr<Node> node);

        // From Updatable
        virtual void Update(float deltaTime);

        // From Drawable
        virtual void Draw(Graphics* graphics, Matrix4x4 worldMatrix);

        Game* GetGame() const { return game_; }
        Camera* GetCamera() const { return camera_; }
        Application* GetApplication() const { return application_; }
        Vector2Int GetResolutionSize() const;

        void SetGame(Game* game);

    protected:
        Scene();

    private:
        Game* game_;
        snowpulse::Application* application_;
        snowpulse::Camera* camera_;
        std::vector<GameObject*> gameObjects_;
        std::vector<Drawable*> drawableChildren_;
    
};
}   // namespace snowpulse
#endif
