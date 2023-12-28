#ifndef SNOWPULSE_GAME_COMPONENTS_BUTTON_H_
#define SNOWPULSE_GAME_COMPONENTS_BUTTON_H_

#include "../../defines.h"
#include "../component.h"

#include <memory>
#include <string>
#include <functional>

#include "../../common/vector2.h"
#include "../../application/input.h"

namespace snowpulse {
class SNOWPULSEAPI Button : public Component {
    public:
        enum class State {
            kIdle = 0,
            kHover,
            kDown,
            kDisabled,
        };

        static std::shared_ptr<Button> Create(Vector2 size);

        virtual ~Button();
        // From Node
        void Start() override;
        // From Updatable
        void Update(float deltaTime) override;

        void SetStateListener(std::function<void(const State& state, bool isPressed)> onStateChanged);

        State GetState() const { return state_; }

    protected:
        Button(std::string componentName);

        bool IsHit();

        Input* input_;
        State state_;
        Vector2 size_;
        bool isPressedDown_;
        std::function<void(const State&, bool)> onStateChanged_;
};
}   // namespace snowpulse

#endif
