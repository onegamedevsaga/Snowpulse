#ifndef SNOWPULSE_GAME_PREFABS_PREFABBUTTON_H_
#define SNOWPULSE_GAME_PREFABS_PREFABBUTTON_H_

#include "../../defines.h"

#include <memory>

#include "../game_object.h"

namespace snowpulse {
class SNOWPULSEAPI PrefabButton {
    public:
        static std::shared_ptr<GameObject> InstantiateSpriteBased(std::string name, Vector2 size=Vector2(150.0f, 50.0f), std::string idleTextureFilename="", std::string downTextureFilename="", std::string hoverTextureFilename="", std::string disabledTextureFilename="");
        static std::shared_ptr<GameObject> InstantiateColorBased(std::string name, std::function<void()> onClick=SPNULL, Vector2 size=Vector2(150.0f, 50.0f), Color idleColor=Color::White(), Color downColor=Color::Green(), Color hoverColor=Color::Blue(), Color disableColor=Color::Red());
};
}   // namespace snowpulse

#endif
