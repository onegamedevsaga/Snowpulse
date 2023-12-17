#include "input_opengl.h"

#include <cctype>

namespace snowpulse {

InputOpenGL::InputOpenGL() : Input() {
    keyMap_[GLFW_KEY_SPACE] = "space";
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
    }
}

InputOpenGL::~InputOpenGL() {
}

void InputOpenGL::ProcessInputs(const Vector2Int& resolutionSize, const Vector2Int& screenSize, GLFWwindow* window) {
    pressedInputs_.clear();
    releasedInputs_.clear();

    for (const auto& pair : keyMap_) {
        if (glfwGetKey(window, pair.first) == GLFW_PRESS) {
            if (canBePressedInputs_.count(pair.second)) {
                pressedInputs_[pair.second];
                canBePressedInputs_.erase(pair.second);
            }
        }
        else if (glfwGetKey(window, pair.first) == GLFW_RELEASE) {
            if (!canBePressedInputs_.count(pair.second)) {
                releasedInputs_[pair.second];
                canBePressedInputs_[pair.second];
            }
        }
    }

    for (const auto& pair : mouseButtonMap_) {
        if (glfwGetMouseButton(window, pair.first) == GLFW_PRESS) {
            if (canBePressedInputs_.count(pair.second)) {
                pressedInputs_[pair.second];
                canBePressedInputs_.erase(pair.second);
            }
        }
        else if (glfwGetMouseButton(window, pair.first) == GLFW_RELEASE) {
            if (!canBePressedInputs_.count(pair.second)) {
                releasedInputs_[pair.second];
                canBePressedInputs_[pair.second];
            }
        }
    }

    double x, y;
    glfwGetCursorPos(window, &x, &y);
    //mousePosition_.x = (float)x;
    //mousePosition_.y = (float)screenSize.y - (float)y;
    Vector2 scaleFactor = Vector2((float)resolutionSize.x / (float)screenSize.x, (float)resolutionSize.y / (float)screenSize.y);
    //mousePosition_ *= scaleFactor;
}

bool InputOpenGL::GetPressed(std::string key) {
    return pressedInputs_.count(ToLowerCase(key));
}

bool InputOpenGL::GetReleased(std::string key) {
    return releasedInputs_.count(ToLowerCase(key));
}
}   // namespace snowpulse
