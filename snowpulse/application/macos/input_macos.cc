#include "input_macos.h"

namespace snowpulse {

InputMacOS::InputMacOS() : Input() {
    /*keyMap_[GLFW_KEY_SPACE] = "space";
    keyMap_[GLFW_KEY_ESCAPE] = "escape";
    keyMap_[GLFW_KEY_ENTER] = "enter";
    keyMap_[GLFW_KEY_KP_ENTER] = "enter";
    keyMap_[GLFW_KEY_UP] = "up_arrow";
    keyMap_[GLFW_KEY_DOWN] = "down_arrow";
    keyMap_[GLFW_KEY_LEFT] = "left_arrow";
    keyMap_[GLFW_KEY_RIGHT] = "right_arrow";
    keyMap_[GLFW_KEY_D] = "d";
    keyMap_[GLFW_KEY_E] = "e";
    keyMap_[GLFW_KEY_F] = "f";
    keyMap_[GLFW_KEY_G] = "g";
    keyMap_[GLFW_KEY_H] = "h";
    keyMap_[GLFW_KEY_I] = "i";
    keyMap_[GLFW_KEY_J] = "j";
    keyMap_[GLFW_KEY_K] = "k";
    keyMap_[GLFW_KEY_L] = "l";
    keyMap_[GLFW_KEY_M] = "m";
    keyMap_[GLFW_KEY_N] = "n";
    keyMap_[GLFW_KEY_O] = "o";
    keyMap_[GLFW_KEY_P] = "p";
    keyMap_[GLFW_KEY_Q] = "q";
    keyMap_[GLFW_KEY_R] = "r";
    keyMap_[GLFW_KEY_S] = "s";
    keyMap_[GLFW_KEY_T] = "t";
    keyMap_[GLFW_KEY_U] = "u";
    keyMap_[GLFW_KEY_V] = "v";
    keyMap_[GLFW_KEY_W] = "w";
    keyMap_[GLFW_KEY_X] = "x";
    keyMap_[GLFW_KEY_Y] = "y";
    keyMap_[GLFW_KEY_Z] = "z";
    keyMap_[GLFW_KEY_0] = "0";
    keyMap_[GLFW_KEY_1] = "1";
    keyMap_[GLFW_KEY_2] = "2";
    keyMap_[GLFW_KEY_3] = "3";
    keyMap_[GLFW_KEY_4] = "4";
    keyMap_[GLFW_KEY_5] = "5";
    keyMap_[GLFW_KEY_6] = "6";
    keyMap_[GLFW_KEY_7] = "7";
    keyMap_[GLFW_KEY_8] = "8";
    keyMap_[GLFW_KEY_9] = "9";

    mouseButtonMap_[GLFW_MOUSE_BUTTON_LEFT] = "mouse_left";
    mouseButtonMap_[GLFW_MOUSE_BUTTON_RIGHT] = "mouse_right";
    mouseButtonMap_[GLFW_MOUSE_BUTTON_MIDDLE] = "mouse_middle";

    for (const auto& pair : keyMap_) {
        canBePressedInputs_[pair.second];
    }

    for (const auto& pair : mouseButtonMap_) {
        canBePressedInputs_[pair.second];
    }*/
}

InputMacOS::~InputMacOS() {
}

void InputMacOS::ProcessInputs(const Vector2Int& resolutionSize, const Vector2Int& screenSize, Vector2 mousePosition) {
    Vector2 scaleFactor = Vector2((float)resolutionSize.x / (float)screenSize.x, (float)resolutionSize.y / (float)screenSize.y);
    mousePositionOnScreen_ = mousePosition;
    mousePositionOnScreen_ *= scaleFactor;

    auto camPos = camera_->GetPosition();
    mousePositionOnWorld_ = mousePositionOnScreen_ + Vector2(camPos.x, camPos.y) + Vector2(resolutionSize.x * -0.5f, resolutionSize.y * -0.5f);
}
void InputMacOS::ProcessInputs(const Vector2Int& resolutionSize, const Vector2Int& screenSize, Vector2 mousePosition, int mouseButton, bool isMouseDown) {
    Vector2 scaleFactor = Vector2((float)resolutionSize.x / (float)screenSize.x, (float)resolutionSize.y / (float)screenSize.y);
    auto mousePositionOnScreen = mousePosition;
    mousePositionOnScreen *= scaleFactor;

    auto camPos = camera_->GetPosition();
    auto mousePositionOnWorld = mousePositionOnScreen + Vector2(camPos.x, camPos.y) + Vector2(resolutionSize.x * -0.5f, resolutionSize.y * -0.5f);
    
    std::string key = "";
    switch (mouseButton) {
        case 0:
            key = "mouse_left";
            break;
        case 1:
            key = "mouse_right";
            break;
        case 2:
            key = "mouse_middle";
            break;
    }
    if (!heldKeys_.count(key) && isMouseDown) {
        pressedKeys_[key] = true;
        heldKeys_[key] = true;
    }
    else if (heldKeys_.count(key) && !isMouseDown) {
        releasedKeys_[key] = true;
        heldKeys_.erase(key);
    }
}
void InputMacOS::ProcessInputs(std::string key, bool isKeyDown) {
    key = ToLowerCase(key);
    if (!heldKeys_.count(key) && isKeyDown) {
        pressedKeys_[key] = true;
        heldKeys_[key] = true;
    }
    else if (heldKeys_.count(key) && !isKeyDown) {
        releasedKeys_[key] = true;
        heldKeys_.erase(key);
    }
}

/*void InputMacOS::ProcessInputs(const Vector2Int& resolutionSize, const Vector2Int& screenSize) {
    

    
}*/

bool InputMacOS::GetPressed(std::string key) {
    return pressedKeys_.count(ToLowerCase(key));
}

bool InputMacOS::GetReleased(std::string key) {
    return releasedKeys_.count(ToLowerCase(key));
}
}   // namespace snowpulse
