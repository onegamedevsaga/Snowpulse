#include "prefab_button.h"

#include <functional>

#include "../components/button.h"
#include "../components/rect_renderer.h"
#include "../components/sprite_renderer.h"

namespace snowpulse {
std::shared_ptr<GameObject> PrefabButton::InstantiateSpriteBased(std::string name, std::function<void()> onClick, Vector2 size, std::string idleTextureFilename, std::string downTextureFilename, std::string hoverTextureFilename, std::string disabledTextureFilename, PathType pathType, bool isDebug) {
    auto baseGo = GameObject::Create(name);
    auto rect = RectRenderer::Create(size);
    auto sprite = SpriteRenderer::Create(idleTextureFilename, pathType);
    auto button = Button::Create(size);
    rect->SetSortOrder(10);
    rect->SetColor(isDebug ? Color(0.0f, 1.0f, 0.0f, 0.5f) : Color::Transparent());
    sprite->SetSortOrder(10);
    button->SetUpdateOrder(10);

    button->SetStateListener([sprite, onClick, idleTextureFilename, downTextureFilename, hoverTextureFilename, pathType](Button::State state, bool isPressed) {
        switch (state) {
            case Button::State::kIdle:
                sprite->SetTextureFilename(idleTextureFilename, pathType);
                break;
            case Button::State::kDown:
                sprite->SetTextureFilename(downTextureFilename, pathType);
                break;
            case Button::State::kHover:
                sprite->SetTextureFilename(hoverTextureFilename, pathType);
                break;
            default:
                break;
        }

        if (isPressed && onClick) {
            onClick();
        }
    });

    baseGo->AddComponent(sprite);
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
