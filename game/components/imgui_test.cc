#include "imgui_test.h"

#include <imgui.h>

namespace game {
std::shared_ptr<ImGUITest> ImGUITest::Create() {
    auto imguiTest = std::shared_ptr<ImGUITest>(new ImGUITest());
    return imguiTest;
}

ImGUITest::ImGUITest() : Component("imgui_test") {
    
}

ImGUITest::~ImGUITest() {
    
}

void ImGUITest::Update(float deltaTime) {
    
}

void ImGUITest::Draw(snowpulse::Graphics* graphics, snowpulse::Matrix4x4 worldMatrix) {
    ImGui::ShowDemoWindow();
}
}   // namespace snowpulse
