#ifndef SNOWPULSE_GAME_GAMEOJBECT_H_
#define SNOWPULSE_GAME_GAMEOJBECT_H_

#include "../defines.h"

#include <memory>
#include <vector>
#include <string>

#include "../application/graphics.h"
#include "../common/matrix4x4.h"
#include "node.h"
#include "updatable.h"
#include "drawable.h"
#include "node_starter.h"
#include "component.h"
#include "components/transform.h"

namespace snowpulse {

class SNOWPULSEAPI GameObject : public Node, public Drawable {
    public:
        static std::shared_ptr<GameObject> Create(std::string name);

        virtual ~GameObject();

        // From Node
        void Start();
        bool AddChild(std::shared_ptr<Node> node);
        bool RemoveChild(std::shared_ptr<Node> node);

        // From Drawable
        void Draw(Graphics* graphics, Matrix4x4 worldMatrix);

        void AddComponent(std::shared_ptr<Component> component);
        void RemoveComponent(std::shared_ptr<Component> component);
        void GetAllComponentsRecursively(std::vector<Component*>& list);

        Transform* GetTransform() { return transform_; }

    protected:
        GameObject(std::string name);

        // From Node
        void QueueForStartPreChildren(NodeStarter& nodeStarter, std::shared_ptr<Node> nodeSmartPtr);

        Transform* transform_;
        std::vector<std::shared_ptr<Component>> components_;
        std::vector<Drawable*> drawableChildren_;
        std::vector<Drawable*> drawableComponents_;
};
}   // namespace snowpulse

#endif
