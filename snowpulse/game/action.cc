#include "action.h"

#include "../application/application.h"

namespace snowpulse {
Action::Action(std::string name) : name_(name), isComplete_(false), onComplete_(SPNULL) {
}

Action::~Action() {
}

void Action::Run() {
    Application::GetInstance()->GetActionManager()->RunAction(this);
}

Action* Action::OnComplete(std::function<void(Action*)> onComplete) {
    if (onComplete) {
        onComplete_ = onComplete;
    }
    return this;
}
}   // namespace snowpulse
