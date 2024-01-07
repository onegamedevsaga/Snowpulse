#include "button.h"

#include "transform.h"
#include "rect_renderer.h"

namespace snowpulse {
std::shared_ptr<Button> Button::Create(Vector2 size) {
    auto button = std::shared_ptr<Button>(new Button("button"));
    button->size_ = size;
    return button;
}

Button::Button(std::string componentName) : Component(componentName), isPressedDown_(false) {
    input_ = Input::GetInstance();
}

Button::~Button() {

}

void Button::Start() {
    auto rect = static_cast<RectRenderer*>(GetComponent("rect_renderer"));
    if (rect) {
        rect->SetSize(size_);
    }
}

// From Updatable
void Button::Update(float deltaTime) {
    Component::Update(deltaTime);

    auto isHit = IsHit();
    if (input_->GetPressed("mouse_left") && IsHit() && (state_ == State::kIdle || state_ == State::kHover)) {
        state_ = State::kDown;
        isPressedDown_ = true;
        if (onStateChanged_) {
            onStateChanged_(state_, false);
        }
    }
    else if (isPressedDown_ && input_->GetReleased("mouse_left")) {
        auto prevState = state_;
#ifdef SNOWPULSE_PLATFORM_IOS || SNOWPULSE_PLATFORM_ANDROID
        state_ = State::kIdle;
#else
        state_ = isHit ? State::kHover : State::kIdle;
#endif
        isPressedDown_ = false;
        if (state_ != prevState) {
            if (onStateChanged_) {
                onStateChanged_(state_, true);
            }
        }
    }
    else {
        auto prevState = state_;
#ifdef SNOWPULSE_PLATFORM_IOS || SNOWPULSE_PLATFORM_ANDROID
        state_ = (!isPressedDown_ || !isHit) ? State::kIdle : State::kDown;
#else
        state_ = isHit ? (isPressedDown_ ? State::kDown : State::kHover) : State::kIdle;
#endif
        if (state_ != prevState) {
            if (onStateChanged_) {
                onStateChanged_(state_, false);
            }
        }
    }

    if (isHit) {
        input_->ConsumeInputs();
    }
}

void Button::SetStateListener(std::function<void(const State& state, bool isPressed)> onStateChanged) {
    onStateChanged_ = onStateChanged;
}

bool Button::IsHit() {
    auto mousePos = input_->GetMousePositionOnWorld();
    auto ownPos = transform_->GetPosition();
    auto halfSize = size_ * 0.5f;
    if (mousePos.x >= ownPos.x - halfSize.x &&
        mousePos.x <= ownPos.x + halfSize.x &&
        mousePos.y >= ownPos.y - halfSize.y &&
        mousePos.y <= ownPos.y + halfSize.y) {
        return true;
    }
    return false;
}
}   // namespace snowpulse
