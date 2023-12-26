#include "action_panel.h"

#include <imgui.h>

namespace game {
std::shared_ptr<ActionPanel> ActionPanel::Create() {
    auto panel = std::shared_ptr<ActionPanel>(new ActionPanel());
    return panel;
}

ActionPanel::ActionPanel() : Component("action_panel") {
}

ActionPanel::~ActionPanel() {
}

void ActionPanel::Update(float deltaTime) {
}

void ActionPanel::Draw(snowpulse::Graphics* graphics, snowpulse::Matrix4x4 worldMatrix) {
    auto windowSize = snowpulse::Application::GetInstance()->GetScreenSize();
    auto panelWidth = windowSize.x;
    ImGuiStyle& style = ImGui::GetStyle();
    auto prevWindowRounding = style.WindowRounding;
    style.WindowRounding = 0.0f;

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(panelWidth, 40.0f));
    ImGui::Begin("Actions", SPNULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    ImGui::End();
    style.WindowRounding = prevWindowRounding;
}
}   // namespace snowpulse
