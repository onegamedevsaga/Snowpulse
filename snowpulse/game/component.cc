#include "component.h"

#include <iostream>

#include "game_object.h"

namespace snowpulse {
Component::Component(std::string name) : Node(name), updateOrder_(0) {
}

Component::~Component() {
}

// From Node
void Component::Start() {
}

bool Component::AddChild(std::shared_ptr<Node> node) {
#ifdef SPDEBUG
    std::cerr << "Error: Component: Can't have children." << std::endl;
#endif
    return false;
}

bool Component::RemoveChild(std::shared_ptr<Node> node) {
#ifdef SPDEBUG
    std::cerr << "Error: Component: Can't have children." << std::endl;
#endif
    return false;
}

// From Updatable
void Component::Update(float deltaTime) {
}

Component* Component::GetComponent(std::string name) const {
    return gameObject_->GetComponent(name);
}
}   // namespace snowpulse
