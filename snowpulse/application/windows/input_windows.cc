#include "input_windows.h"

#include <imgui.h>
#include <GLFW/glfw3.h>

#include "../application.h"

namespace snowpulse {

InputWindows::InputWindows() : Input() {
    keyMap_[GLFW_KEY_LEFT_SUPER] = "super";
    keyMap_[GLFW_KEY_LEFT_CONTROL] = "control";
    keyMap_[GLFW_KEY_LEFT_SHIFT] = "shift";
    keyMap_[GLFW_KEY_TAB] = "tab";
    keyMap_[GLFW_KEY_LEFT] = "left_arrow";
    keyMap_[GLFW_KEY_RIGHT] = "right_arrow";
    keyMap_[GLFW_KEY_UP] = "up_arrow";
    keyMap_[GLFW_KEY_DOWN] = "down_arrow";
    keyMap_[GLFW_KEY_PAGE_UP] = "page_up";
    keyMap_[GLFW_KEY_PAGE_DOWN] = "page_down";
    keyMap_[GLFW_KEY_HOME] = "home";
    keyMap_[GLFW_KEY_END] = "end";
    keyMap_[GLFW_KEY_BACKSPACE] = "backspace";
    keyMap_[GLFW_KEY_DELETE] = "delete";
    keyMap_[GLFW_KEY_RIGHT] = "space";
    keyMap_[GLFW_KEY_ENTER] = "enter";
    keyMap_[GLFW_KEY_ESCAPE] = "escape";
    keyMap_[GLFW_KEY_KP_ENTER] = "keypad_enter";
    keyMap_[GLFW_KEY_A] = "a";
    keyMap_[GLFW_KEY_B] = "b";
    keyMap_[GLFW_KEY_C] = "c";
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

    ImGuiIO& io = ImGui::GetIO();
    io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = kVK_LeftArrow;
    io.KeyMap[ImGuiKey_RightArrow] = kVK_RightArrow;
    io.KeyMap[ImGuiKey_UpArrow] = kVK_UpArrow;
    io.KeyMap[ImGuiKey_DownArrow] = kVK_DownArrow;
    io.KeyMap[ImGuiKey_PageUp] = kVK_PageUp;
    io.KeyMap[ImGuiKey_PageDown] = kVK_PageDown;
    io.KeyMap[ImGuiKey_Home] = kVK_Home;
    io.KeyMap[ImGuiKey_End] = kVK_End;
    io.KeyMap[ImGuiKey_Delete] = kVK_Delete;
    io.KeyMap[ImGuiKey_Backspace] = kVK_Delete;
    io.KeyMap[ImGuiKey_Space] = kVK_Space;
    io.KeyMap[ImGuiKey_Enter] = kVK_Return;
    io.KeyMap[ImGuiKey_Escape] = kVK_Escape;
    io.KeyMap[ImGuiKey_KeyPadEnter] = kVK_ANSI_KeypadEnter;
    io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
    io.KeyMap[ImGuiKey_B] = GLFW_KEY_B;
    io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
    io.KeyMap[ImGuiKey_D] = GLFW_KEY_D;
    io.KeyMap[ImGuiKey_E] = GLFW_KEY_E;
    io.KeyMap[ImGuiKey_F] = GLFW_KEY_F;
    io.KeyMap[ImGuiKey_G] = GLFW_KEY_G;
    io.KeyMap[ImGuiKey_H] = GLFW_KEY_H;
    io.KeyMap[ImGuiKey_I] = GLFW_KEY_I;
    io.KeyMap[ImGuiKey_J] = GLFW_KEY_J;
    io.KeyMap[ImGuiKey_K] = GLFW_KEY_K;
    io.KeyMap[ImGuiKey_L] = GLFW_KEY_L;
    io.KeyMap[ImGuiKey_M] = GLFW_KEY_M;
    io.KeyMap[ImGuiKey_N] = GLFW_KEY_N;
    io.KeyMap[ImGuiKey_O] = GLFW_KEY_O;
    io.KeyMap[ImGuiKey_P] = GLFW_KEY_P;
    io.KeyMap[ImGuiKey_Q] = GLFW_KEY_Q;
    io.KeyMap[ImGuiKey_R] = GLFW_KEY_R;
    io.KeyMap[ImGuiKey_S] = GLFW_KEY_S;
    io.KeyMap[ImGuiKey_T] = GLFW_KEY_T;
    io.KeyMap[ImGuiKey_U] = GLFW_KEY_U;
    io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
    io.KeyMap[ImGuiKey_W] = GLFW_KEY_W;
    io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
    io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
    io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;
    io.KeyMap[ImGuiKey_0] = GLFW_KEY_0;
    io.KeyMap[ImGuiKey_1] = GLFW_KEY_1;
    io.KeyMap[ImGuiKey_2] = GLFW_KEY_2;
    io.KeyMap[ImGuiKey_3] = GLFW_KEY_3;
    io.KeyMap[ImGuiKey_4] = GLFW_KEY_4;
    io.KeyMap[ImGuiKey_5] = GLFW_KEY_5;
    io.KeyMap[ImGuiKey_6] = GLFW_KEY_6;
    io.KeyMap[ImGuiKey_7] = GLFW_KEY_7;
    io.KeyMap[ImGuiKey_8] = GLFW_KEY_8;
    io.KeyMap[ImGuiKey_9] = GLFW_KEY_9;
}

InputMacOS::~InputMacOS() {
}

void InputMacOS::ProcessInputs(const Vector2Int& resolutionSize, const Vector2Int& screenSize, Vector2 mousePosition) {
    Vector2 scaleFactor = Vector2((float)resolutionSize.x / (float)screenSize.x, (float)resolutionSize.y / (float)screenSize.y);
    mousePositionOnScreen_ = mousePosition;
    mousePositionOnScreen_ *= scaleFactor;

    auto camPos = camera_->GetPosition();
    mousePositionOnWorld_ = mousePositionOnScreen_ + Vector2(camPos.x, camPos.y) + Vector2(resolutionSize.x * -0.5f, resolutionSize.y * -0.5f);

    ImGuiIO& io = ImGui::GetIO();
    io.MouseDrawCursor = true;
    io.MousePos = ImVec2(mousePosition.x, screenSize.y - mousePosition.y);
}
void InputMacOS::ProcessInputs(const Vector2Int& resolutionSize, const Vector2Int& screenSize, Vector2 mousePosition, int mouseButton, bool isMouseDown) {
    Vector2 scaleFactor = Vector2((float)resolutionSize.x / (float)screenSize.x, (float)resolutionSize.y / (float)screenSize.y);
    auto mousePositionOnScreen = mousePosition;
    mousePositionOnScreen *= scaleFactor;

    auto camPos = camera_->GetPosition();
    mousePositionOnWorld_ = mousePositionOnScreen + Vector2(camPos.x, camPos.y) + Vector2(resolutionSize.x * -0.5f, resolutionSize.y * -0.5f);

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

    ImGuiIO& io = ImGui::GetIO();
    io.MouseDown[mouseButton] = isMouseDown;
    io.MousePos = ImVec2(mousePosition.x, screenSize.y - mousePosition.y);
}

void InputMacOS::ProcessInputs(unsigned int keyCode, bool isKeyDown) {
    auto key = keyMap_[keyCode];
    if (!heldKeys_.count(key) && isKeyDown) {
        pressedKeys_[key] = true;
        heldKeys_[key] = true;
    }
    else if (heldKeys_.count(key) && !isKeyDown) {
        releasedKeys_[key] = true;
        heldKeys_.erase(key);
    }

    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[keyCode] = isKeyDown;
    io.KeySuper = heldKeys_.count("super") == 1;
    io.KeyAlt = heldKeys_.count("option") == 1;
    io.KeyCtrl = heldKeys_.count("control") == 1;
    io.KeyShift = heldKeys_.count("shift") == 1;
}

void InputMacOS::ProcessInputs(std::string characters) {
    ImGuiIO& io = ImGui::GetIO();
    for (int i = 0; i < (int)characters.size(); i++) {
        auto c = characters[i];
        if ((int)c > 0) {
            io.AddInputCharacter((unsigned short)c);
        }
    }
}

void InputMacOS::ProcessInputs(const Vector2& scrollDelta) {
    scrollDelta_ = scrollDelta;
    float scaleFactor = application_->GetScaleFactor() * 10.0f;
    ImGuiIO& io = ImGui::GetIO();
    io.MouseWheelH += scrollDelta_.x / scaleFactor;
    io.MouseWheel += scrollDelta_.y / scaleFactor;
}

void InputMacOS::ClearLastFrameData() {
    Input::ClearLastFrameData();
    ImGuiIO& io = ImGui::GetIO();
    for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) {
        io.KeysDown[i] = false;
    }
}

bool InputMacOS::GetPressed(std::string key) {
    return pressedKeys_.count(ToLowerCase(key));
}

bool InputMacOS::GetDown(std::string key) {
    return heldKeys_.count(ToLowerCase(key));
}

bool InputMacOS::GetReleased(std::string key) {
    return releasedKeys_.count(ToLowerCase(key));
}
}   // namespace snowpulse
