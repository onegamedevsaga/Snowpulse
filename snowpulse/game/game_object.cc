#include "game_object.h"

#include "../application/application.h"
#include "scene.h"

namespace snowpulse {

std::shared_ptr<GameObject> GameObject::Create(std::string name) {
    auto gameObject = std::shared_ptr<GameObject>(new GameObject(name));
    return gameObject;
}

GameObject::GameObject(std::string name) : Node ("game_object: " + name) {
    auto transform = snowpulse::Transform::Create();
    transform_ = transform.get();
    AddComponent(transform);
}

GameObject::~GameObject() {
}

// From Node
void GameObject::Start() {
}

bool GameObject::AddChild(std::shared_ptr<Node> node) {
    auto gameObject = dynamic_cast<GameObject*>(node.get());
    if (gameObject) {
        if (Node::AddChild(node)) {
            auto updatable = dynamic_cast<Updatable*>(node.get());
            auto drawable = dynamic_cast<Drawable*>(node.get());

            if (updatable) {
                updatableChildren_.push_back(updatable);
            }
            if (drawable) {
                drawableChildren_.push_back(drawable);
            }
            return true;
        }
    }
    return false;
}

bool GameObject::RemoveChild(std::shared_ptr<Node> node) {
    if (Node::RemoveChild(node)) {
        auto updatable = dynamic_cast<Updatable*>(node.get());
        if (updatable) {
            for (int i = 0; i < updatableChildren_.size(); i++) {
                if (updatableChildren_[i] == updatable) {
                    updatableChildren_.erase(updatableChildren_.begin() + i);
                    break;
                }
            }
        }

        auto drawable = dynamic_cast<Drawable*>(node.get());
        if (drawable) {
            for (int i = 0; i < drawableChildren_.size(); i++) {
                if (drawableChildren_[i] == drawable) {
                    drawableChildren_.erase(drawableChildren_.begin() + i);
                    break;
                }
            }
        }
        return true;
    }
    return false;
}

// From Updatable
void GameObject::Update(float deltaTime) {
    for (auto& c : components_) {
        c->Update(deltaTime);
    }

    for (auto u : updatableChildren_) {
        u->Update(deltaTime);
    }
}

// From Drawable
void GameObject::Draw(Graphics* graphics, Matrix4x4 worldMatrix) {
    Matrix4x4 matrix = transform_->GetLocalMatrix() * worldMatrix;

    for (auto d : drawableComponents_) {
        d->Draw(graphics, matrix);
    }

    for (auto d : drawableChildren_) {
        d->Draw(graphics, matrix);
    }
}

void GameObject::AddComponent(std::shared_ptr<Component> component) {
    for (const auto& c : components_) {
        if (c.get() == component.get()) {
            return;
        }
    }
    component->gameObject_ = this;
    component->transform_ = transform_;
    components_.push_back(component);

    auto d = dynamic_cast<Drawable*>(component.get());
    if (d != nullptr) {
        drawableComponents_.push_back(d);
    }
}

void GameObject::RemoveComponent(std::shared_ptr<Component> component) {
    auto drawable = dynamic_cast<Drawable*>(component.get());
    if (drawable) {
        for (int i = 0; i < drawableComponents_.size(); i++) {
            if (drawableComponents_[i] == drawable) {
                drawableComponents_.erase(drawableComponents_.begin() + i);
                break;
            }
        }
    }

    for (int i = 0; i < components_.size(); i++) {
        if (components_[i].get() == component.get()) {
            components_.erase(components_.begin() + i);
            break;
        }
    }
}

void GameObject::QueueForStartPreChildren(NodeStarter& nodeStarter, std::shared_ptr<Node> nodeSmartPtr) {
    for (auto& c : components_) {
        nodeStarter.AddNode(c);
    }
}
}   // namespace snowpulse
