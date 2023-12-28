#include "prefab_button.h"

#include <functional>

#include "../components/button.h"
#include "../components/rect_renderer.h"
#include "../components/sprite_renderer.h"

namespace snowpulse {
std::shared_ptr<GameObject> PrefabButton::InstantiateSpriteBased(std::string name, Vector2 size, std::string idleTextureFilename, std::string downTextureFilename, std::string hoverTextureFilename, std::string disabledTextureFilename) {
    auto baseGo = GameObject::Create(name);
    auto rect = RectRenderer::Create(size);
    auto button = Button::Create(size);
    rect->SetSortOrder(10);
    rect->SetColor(Color::Transparent());
    button->SetUpdateOrder(10);

    baseGo->AddComponent(rect);
    baseGo->AddComponent(button);
    return baseGo;
}

std::shared_ptr<GameObject> PrefabButton::InstantiateColorBased(std::string name, std::function<void()> onClick, Vector2 size, Color idleColor, Color downColor, Color hoverColor, Color disableColor) {
    auto baseGo = GameObject::Create(name);
    auto rect = RectRenderer::Create(size);
    auto button = Button::Create(size);
    rect->SetSortOrder(10);
    rect->SetColor(idleColor);
    button->SetUpdateOrder(10);

    button->SetStateListener([rect, onClick, idleColor, downColor, hoverColor](Button::State state, bool isPressed) {
        switch (state) {
            case Button::State::kIdle:
                rect->SetColor(idleColor);
                break;
            case Button::State::kDown:
                rect->SetColor(downColor);
                break;
            case Button::State::kHover:
                rect->SetColor(hoverColor);
                break;
            default:
                break;
        }

        if (isPressed && onClick) {
            onClick();
        }
    });

    baseGo->AddComponent(rect);
    baseGo->AddComponent(button);
    return baseGo;
}
}   // namespace snowpulse
