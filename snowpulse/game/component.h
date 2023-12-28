#ifndef SNOWPULSE_GAME_COMPONENT_H_
#define SNOWPULSE_GAME_COMPONENT_H_

#include "../defines.h"

#include <memory>

#include "node.h"
#include "updatable.h"

namespace snowpulse {

// Forward declaration
class GameObject;
class Transform;

class SNOWPULSEAPI Component : public Node, public Updatable {
    public:
        virtual ~Component();

        // From Node
        virtual void Start();
        bool AddChild(std::shared_ptr<Node> node);
        bool RemoveChild(std::shared_ptr<Node> node);

        // From Updatable
        virtual void Update(float deltaTime);

        void SetUpdateOrder(int updateOrder) { updateOrder_ = updateOrder; }

        int GetUpdateOrder() const { return updateOrder_; }
        GameObject* GetGameObject() const { return gameObject_; }
        Transform* GetTransform() const { return transform_; }

    protected:
        friend class GameObject;

        Component(std::string name);

        int updateOrder_;
        GameObject* gameObject_;
        Transform* transform_;
    
    private:
        Component() : Node("#") { }
};
}   // namespace snowpulse

#endif
