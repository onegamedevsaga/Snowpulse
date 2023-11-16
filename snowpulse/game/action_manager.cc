#include "action_manager.h"

#include <memory>

namespace snowpulse {
std::shared_ptr<ActionManager> ActionManager::Create() {
    auto actionMgr = std::shared_ptr<ActionManager>(new ActionManager());
    return actionMgr;
}

ActionManager::ActionManager() {
}

ActionManager::~ActionManager() {
}

void ActionManager::Start() {
}

void ActionManager::Update(float deltaTime) {
    auto copy = std::vector<std::shared_ptr<Action>>(actions_);
    for (int i = 0; i < (int)copy.size(); i++) {
        auto a = copy[i];
        a->Update(deltaTime);
        if (a->IsComplete()) {
            RemoveAction(a.get());
        }
    }
}

void ActionManager::RunAction(Action* action) {
    actions_.push_back(std::shared_ptr<Action>(action));
}

void ActionManager::RemoveAction(Action* action) {
    for (int i = 0; i < (int)actions_.size(); i++) {
        auto a = actions_[i].get();
        if (a == action) {
            actions_.erase(actions_.begin() + i);
            break;
        }
    }
}
}   // namespace snowpulse
