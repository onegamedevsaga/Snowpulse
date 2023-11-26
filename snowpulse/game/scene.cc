#include "scene.h"

#include <iostream>

#include "../application/application.h"

namespace snowpulse {
Scene::Scene() : Node("scene") {
}

Scene::~Scene() {
}

void Scene::Initialize(Application* application) {
    application_ = application;
    game_ = application_->GetGame();
    camera_ = application_->GetGraphics()->GetCamera();
}

void Scene::Shutdown() {
    children_.clear();
#ifdef SPDEBUG
    std::cout << "Scene shutdown." << std::endl;
#endif
}

// From Node
void Scene::Start() {
    
#ifdef SPDEBUG
    std::cout << "Scene started.." << std::endl;
#endif
}

bool Scene::AddChild(std::shared_ptr<Node> node) {
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

bool Scene::RemoveChild(std::shared_ptr<Node> node) {
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
void Scene::Update(float deltaTime) {
    for (auto u : updatableChildren_) {
        u->Update(deltaTime);
    }
#ifdef SPDEBUG
    std::cout << "Scene updating.." << std::endl;
#endif
}

// From Drawable
void Scene::Draw(Graphics* graphics, Matrix4x4 worldMatrix) {
    for (auto d : drawableChildren_) {
        d->Draw(graphics, worldMatrix);
    }
#ifdef SPDEBUG
    std::cout << "Scene drawing.." << std::endl;
#endif
}

void Scene::SetGame(Game* game) {
    game_ = game;
}
}   // namespace snowpulse
