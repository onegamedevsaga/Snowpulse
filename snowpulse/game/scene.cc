#include "scene.h"

#include <iostream>

#include "../application/application.h"
#include "component.h"

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
            gameObjects_.push_back(gameObject);

            auto drawable = dynamic_cast<Drawable*>(node.get());
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
        auto gameObject = dynamic_cast<GameObject*>(node.get());
        if (gameObject) {
            for (int i = 0; i < gameObjects_.size(); i++) {
                if (gameObjects_[i] == gameObject) {
                    gameObjects_.erase(gameObjects_.begin() + i);
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
    std::vector<Component*> components;
    components.reserve(512);
    for (auto g : gameObjects_) {
        g->GetAllComponentsRecursively(components);
    }

    std::sort(components.begin(), components.end(), [](auto a, auto b) {
        return a->GetUpdateOrder() > b->GetUpdateOrder();
    });

    for (auto c : components) {
        c->Update(deltaTime);
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

Vector2Int Scene::GetResolutionSize() const {
    return application_->GetResolutionSize();
}

void Scene::SetGame(Game* game) {
    game_ = game;
}
}   // namespace snowpulse
