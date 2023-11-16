#include "node_starter.h"

namespace snowpulse {
NodeStarter::NodeStarter() {
}

NodeStarter::~NodeStarter() {
}

void NodeStarter::AddNode(std::shared_ptr<Node> node) {
    nodes_.push(node);
}

void NodeStarter::StartNodes() {
    while (!nodes_.empty()) {
        const auto& n = nodes_.front();
        n->Start();
        nodes_.pop();
    }
}
}   // namespace snowpulse
