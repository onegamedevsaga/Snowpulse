#include "component.h"

#include <iostream>

namespace snowpulse {
Component::Component(std::string name) : Node(name) {
}

Component::~Component() {
}

// From Node
void Component::Start() {
}

bool Component::AddChild(std::shared_ptr<Node> node) {
#ifdef SPDEBUG
    std::cout << "Error: Components can't have children." << std::endl;
#endif
    return false;
}

bool Component::RemoveChild(std::shared_ptr<Node> node) {
#ifdef SPDEBUG
    std::cout << "Error: Components can't have children." << std::endl;
#endif
    return false;
}

// From Updatable
void Component::Update(float deltaTime) {
}
}   // namespace snowpulse
