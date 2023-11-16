#ifndef SNOWPULSE_GAME_NODE_H_
#define SNOWPULSE_GAME_NODE_H_

#include "../defines.h"

#include <memory>
#include <vector>
#include <string>

namespace snowpulse {

// Forward declaration
class Game;
class NodeStarter;

class SNOWPULSEAPI Node {
    public:
        virtual ~Node();
        virtual void Start() = 0;

        std::string GetName() { return name_; }
        Node* GetParent() { return parent_; }
        std::vector<std::shared_ptr<Node>> GetChildren() const { return children_; };

        virtual bool AddChild(std::shared_ptr<Node> node);
        virtual bool RemoveChild(std::shared_ptr<Node> node);

    protected:
        virtual void QueueForStartPreChildren(NodeStarter& nodeStarter, std::shared_ptr<Node> nodeSmartPtr) { }

        Node(std::string name);
        bool IsConnectedToGame(Game* game);

        Node* parent_;
        std::vector<std::shared_ptr<Node>> children_;

    private:
        Node() { }
        void QueueForStart(NodeStarter& nodeStarter, std::shared_ptr<Node> nodeSmartPtr);

        std::string name_;
};
}   // namespace snowpulse
#endif
