#include "node.h"

#include <iostream>

#include "../application/application.h"
#include "node_starter.h"

namespace snowpulse {
#ifdef SPDEBUG
static int nodeCount = 0;
#endif
Node::Node(std::string name) : parent_(SPNULL), name_(name) {
#ifdef SPDEBUG
    nodeCount ++;
    std::cout << "Node Total : " << nodeCount << " (" << name_ << ")" << std::endl;
#endif
}

Node::~Node() {
#ifdef SPDEBUG
    nodeCount --;
    std::cout << "Node Total : " << nodeCount << " (" << name_ << ")" << std::endl;
#endif
}

bool Node::AddChild(std::shared_ptr<Node> node) {
    for (auto& c : children_) {
        if (c.get() == node.get()) {
            return false;
        }
    }
    auto game = Application::GetInstance()->GetGame();
    if (IsConnectedToGame(game)) {
        node->QueueForStart(Application::GetInstance()->GetNodeStarter(), node);
    }

    node->parent_ = this;
    children_.push_back(node);
    return true;
}

bool Node::RemoveChild(std::shared_ptr<Node> node) {
    for (auto i = 0; i < children_.size(); i++) {
        if (children_[i].get() == node.get()) {
            children_.erase(children_.begin() + i);
            return true;
        }
    }
    return false;
}

bool Node::IsConnectedToGame(Game* game) {
    auto parent = parent_;
    while (parent != SPNULL) {
        if (parent == game) {
            return true;
        }
        parent = parent->parent_;
    }
    return false;
}

void Node::QueueForStart(NodeStarter& nodeStarter, std::shared_ptr<Node> nodeSmartPtr) {
    nodeStarter.AddNode(nodeSmartPtr);

    QueueForStartPreChildren(nodeStarter, nodeSmartPtr);

    for (auto& c : children_) {
        c->QueueForStart(nodeStarter, nodeSmartPtr);
    }
}
}   // namespace snowpulse
