#include "input_macos.h"

#include <imgui.h>
#include <Carbon/Carbon.h>

#include "../application.h"

namespace snowpulse {

InputMacOS::InputMacOS() : Input() {
    keyMap_[kVK_Command] = "super";
    keyMap_[kVK_Option] = "option";
    keyMap_[kVK_Control] = "control";
    keyMap_[kVK_Shift] = "shift";
    keyMap_[kVK_Tab] = "tab";
    keyMap_[kVK_ANSI_LeftBracket] = "left_bracket";
    keyMap_[kVK_ANSI_RightBracket] = "right_bracket";
    keyMap_[kVK_LeftArrow] = "left_arrow";
    keyMap_[kVK_RightArrow] = "right_arrow";
    keyMap_[kVK_UpArrow] = "up_arrow";
    keyMap_[kVK_DownArrow] = "down_arrow";
    keyMap_[kVK_PageUp] = "page_up";
    keyMap_[kVK_PageDown] = "page_down";
    keyMap_[kVK_Home] = "home";
    keyMap_[kVK_End] = "end";
    keyMap_[kVK_Delete] = "delete";
    keyMap_[kVK_Space] = "space";
    keyMap_[kVK_Return] = "enter";
    keyMap_[kVK_Escape] = "escape";
    keyMap_[kVK_ANSI_KeypadEnter] = "keypad_enter";
    keyMap_[kVK_ANSI_A] = "a";
    keyMap_[kVK_ANSI_B] = "b";
    keyMap_[kVK_ANSI_C] = "c";
    keyMap_[kVK_ANSI_D] = "d";
    keyMap_[kVK_ANSI_E] = "e";
    keyMap_[kVK_ANSI_F] = "f";
    keyMap_[kVK_ANSI_G] = "g";
    keyMap_[kVK_ANSI_H] = "h";
    keyMap_[kVK_ANSI_I] = "i";
    keyMap_[kVK_ANSI_J] = "j";
    keyMap_[kVK_ANSI_K] = "k";
    keyMap_[kVK_ANSI_L] = "l";
    keyMap_[kVK_ANSI_M] = "m";
    keyMap_[kVK_ANSI_N] = "n";
    keyMap_[kVK_ANSI_O] = "o";
    keyMap_[kVK_ANSI_P] = "p";
    keyMap_[kVK_ANSI_Q] = "q";
    keyMap_[kVK_ANSI_R] = "r";
    keyMap_[kVK_ANSI_S] = "s";
    keyMap_[kVK_ANSI_T] = "t";
    keyMap_[kVK_ANSI_U] = "u";
    keyMap_[kVK_ANSI_V] = "v";
    keyMap_[kVK_ANSI_W] = "w";
    keyMap_[kVK_ANSI_X] = "x";
    keyMap_[kVK_ANSI_Y] = "y";
    keyMap_[kVK_ANSI_Z] = "z";
    keyMap_[kVK_ANSI_0] = "0";
    keyMap_[kVK_ANSI_1] = "1";
    keyMap_[kVK_ANSI_2] = "2";
    keyMap_[kVK_ANSI_3] = "3";
    keyMap_[kVK_ANSI_4] = "4";
    keyMap_[kVK_ANSI_5] = "5";
    keyMap_[kVK_ANSI_6] = "6";
    keyMap_[kVK_ANSI_7] = "7";
    keyMap_[kVK_ANSI_8] = "8";
    keyMap_[kVK_ANSI_9] = "9";

    ImGuiIO& io = ImGui::GetIO();
    io.KeyMap[ImGuiKey_Tab] = kVK_Tab;
    io.KeyMap[ImGuiKey_LeftBracket] = kVK_ANSI_LeftBracket;
    io.KeyMap[ImGuiKey_RightBracket] = kVK_ANSI_RightBracket;
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
    io.KeyMap[ImGuiKey_A] = kVK_ANSI_A;
    io.KeyMap[ImGuiKey_B] = kVK_ANSI_B;
    io.KeyMap[ImGuiKey_C] = kVK_ANSI_C;
    io.KeyMap[ImGuiKey_D] = kVK_ANSI_D;
    io.KeyMap[ImGuiKey_E] = kVK_ANSI_E;
    io.KeyMap[ImGuiKey_F] = kVK_ANSI_F;
    io.KeyMap[ImGuiKey_G] = kVK_ANSI_G;
    io.KeyMap[ImGuiKey_H] = kVK_ANSI_H;
    io.KeyMap[ImGuiKey_I] = kVK_ANSI_I;
    io.KeyMap[ImGuiKey_J] = kVK_ANSI_J;
    io.KeyMap[ImGuiKey_K] = kVK_ANSI_K;
    io.KeyMap[ImGuiKey_L] = kVK_ANSI_L;
    io.KeyMap[ImGuiKey_M] = kVK_ANSI_M;
    io.KeyMap[ImGuiKey_N] = kVK_ANSI_N;
    io.KeyMap[ImGuiKey_O] = kVK_ANSI_O;
    io.KeyMap[ImGuiKey_P] = kVK_ANSI_P;
    io.KeyMap[ImGuiKey_Q] = kVK_ANSI_Q;
    io.KeyMap[ImGuiKey_R] = kVK_ANSI_R;
    io.KeyMap[ImGuiKey_S] = kVK_ANSI_S;
    io.KeyMap[ImGuiKey_T] = kVK_ANSI_T;
    io.KeyMap[ImGuiKey_U] = kVK_ANSI_U;
    io.KeyMap[ImGuiKey_V] = kVK_ANSI_V;
    io.KeyMap[ImGuiKey_W] = kVK_ANSI_W;
    io.KeyMap[ImGuiKey_X] = kVK_ANSI_X;
    io.KeyMap[ImGuiKey_Y] = kVK_ANSI_Y;
    io.KeyMap[ImGuiKey_Z] = kVK_ANSI_Z;
    io.KeyMap[ImGuiKey_0] = kVK_ANSI_0;
    io.KeyMap[ImGuiKey_1] = kVK_ANSI_1;
    io.KeyMap[ImGuiKey_2] = kVK_ANSI_2;
    io.KeyMap[ImGuiKey_3] = kVK_ANSI_3;
    io.KeyMap[ImGuiKey_4] = kVK_ANSI_4;
    io.KeyMap[ImGuiKey_5] = kVK_ANSI_5;
    io.KeyMap[ImGuiKey_6] = kVK_ANSI_6;
    io.KeyMap[ImGuiKey_7] = kVK_ANSI_7;
    io.KeyMap[ImGuiKey_8] = kVK_ANSI_8;
    io.KeyMap[ImGuiKey_9] = kVK_ANSI_9;
}

InputMacOS::~InputMacOS() {
}

void InputMacOS::ProcessInputs(const Vector2Int& resolutionSize, const Vector2Int& screenSize, Vector2 mousePosition) {
    Vector2 scaleFactor = Vector2((float)resolutionSize.x / (float)screenSize.x, (float)resolutionSize.y / (float)screenSize.y);
    mousePositionOnScreen_ = mousePosition;
    prevMousePositionOnScreen_ = mousePositionOnScreen_;

    auto camPos = camera_->GetPosition();
    mousePositionOnWorld_ = (mousePositionOnScreen_ * scaleFactor) + Vector2(camPos.x, camPos.y) + Vector2(resolutionSize.x * -0.5f, resolutionSize.y * -0.5f);
    prevMousePositionOnWorld_ = mousePositionOnWorld_;

    ImGuiIO& io = ImGui::GetIO();
    io.MouseDrawCursor = false;
    io.MousePos = ImVec2(mousePosition.x, screenSize.y - mousePosition.y);
}

void InputMacOS::ProcessInputs(const Vector2Int& resolutionSize, const Vector2Int& screenSize, Vector2 mousePosition, int mouseButton, bool isMouseDown) {
    Vector2 scaleFactor = Vector2((float)resolutionSize.x / (float)screenSize.x, (float)resolutionSize.y / (float)screenSize.y);
    mousePositionOnScreen_ = mousePosition;
    prevMousePositionOnScreen_ = mousePositionOnScreen_;

    auto camPos = camera_->GetPosition();
    mousePositionOnWorld_ = (mousePositionOnScreen_ * scaleFactor) + Vector2(camPos.x, camPos.y) + Vector2(resolutionSize.x * -0.5f, resolutionSize.y * -0.5f);
    prevMousePositionOnWorld_ = mousePositionOnWorld_;

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
        if (key == "super" || key == "option" || key == "control") {
            for (const auto& pair : heldKeys_) {
                releasedKeys_[pair.first] = true;
            }
            heldKeys_.clear();
        }
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
