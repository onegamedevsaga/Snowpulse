#include "inspector_panel.h"

#include <imgui.h>

namespace game {
std::shared_ptr<InspectorPanel> InspectorPanel::Create() {
    auto panel = std::shared_ptr<InspectorPanel>(new InspectorPanel());
    return panel;
}

InspectorPanel::InspectorPanel() : Component("inspector_panel") {
    valueModes_.reserve(3);
    valueModes_.push_back(GetValueModeString(snowpulse::ParticleSystemSettings::ValueMode::kSingle));
    valueModes_.push_back(GetValueModeString(snowpulse::ParticleSystemSettings::ValueMode::kRandomBetween));
    valueModes_.push_back(GetValueModeString(snowpulse::ParticleSystemSettings::ValueMode::kRandomWithin));
    emissionShapes_.reserve(3);
    emissionShapes_.push_back(GetEmissionShapeString(snowpulse::ParticleSystemSettings::EmissionShape::kPoint));
    emissionShapes_.push_back(GetEmissionShapeString(snowpulse::ParticleSystemSettings::EmissionShape::kRect));
    emissionShapes_.push_back(GetEmissionShapeString(snowpulse::ParticleSystemSettings::EmissionShape::kCircle));
}

InspectorPanel::~InspectorPanel() {
    
}

void InspectorPanel::Update(float deltaTime) {
    settings_.emissionShape = (snowpulse::ParticleSystemSettings::EmissionShape)emissionShape_;
    settings_.lifespanValueMode = (snowpulse::ParticleSystemSettings::ValueMode)lifespanValueMode_;
}

void InspectorPanel::Draw(snowpulse::Graphics* graphics, snowpulse::Matrix4x4 worldMatrix) {
    auto windowHeight = snowpulse::Application::GetInstance()->GetScreenSize().y;
    ImGui::SetNextWindowPos(ImVec2(0, 0)); // Position at the top-left corner
    ImGui::SetNextWindowSize(ImVec2(300, windowHeight)); // Width of 300 and height matching the application window
    ImGui::Begin("Inspector", SPNULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    DrawCombo("Emission Shape", &emissionShape_, emissionShapes_);
    if (settings_.emissionShape == snowpulse::ParticleSystemSettings::EmissionShape::kPoint) {
    }
    else if (settings_.emissionShape == snowpulse::ParticleSystemSettings::EmissionShape::kRect) {
        DrawInputSize("Rect Size", &settings_.emissionRectSize.x, &settings_.emissionRectSize.y);
    }
    else if (settings_.emissionShape == snowpulse::ParticleSystemSettings::EmissionShape::kCircle) {
        DrawInputFloat("SpeedFrom", &settings_.speedA);
        DrawInputFloat("SpeedTo", &settings_.speedB);
    }

    DrawCombo("Lifespan Value Mode", &lifespanValueMode_, valueModes_);
    if (settings_.lifespanValueMode == snowpulse::ParticleSystemSettings::ValueMode::kSingle) {
        DrawInputFloat("Lifespan", &settings_.lifespanA);
    }
    else if (settings_.lifespanValueMode == snowpulse::ParticleSystemSettings::ValueMode::kRandomBetween) {
        DrawInputFloat("LifespanA", &settings_.lifespanA);
        DrawInputFloat("LifespanB", &settings_.lifespanB);
    }
    else if (settings_.lifespanValueMode == snowpulse::ParticleSystemSettings::ValueMode::kRandomWithin) {
        DrawInputFloat("LifespanFrom", &settings_.lifespanA);
        DrawInputFloat("LifespanTo", &settings_.lifespanB);
    }

    DrawCombo("Speed Value Mode", &speedValueMode_, valueModes_);
    if (settings_.speedValueMode == snowpulse::ParticleSystemSettings::ValueMode::kSingle) {
        DrawInputFloat("Speed", &settings_.speedA);
    }
    else if (settings_.speedValueMode == snowpulse::ParticleSystemSettings::ValueMode::kRandomBetween) {
        DrawInputFloat("SpeedA", &settings_.speedA);
        DrawInputFloat("SpeedB", &settings_.speedB);
    }
    else if (settings_.speedValueMode == snowpulse::ParticleSystemSettings::ValueMode::kRandomWithin) {
        DrawInputFloat("SpeedFrom", &settings_.speedA);
        DrawInputFloat("SpeedTo", &settings_.speedB);
    }

    ImGui::End();
}

std::string InspectorPanel::GetValueModeString(snowpulse::ParticleSystemSettings::ValueMode valueMode) {
    switch (valueMode) {
        case snowpulse::ParticleSystemSettings::ValueMode::kSingle:
            return "Single";
        case snowpulse::ParticleSystemSettings::ValueMode::kRandomBetween:
            return "Random Between";
        case snowpulse::ParticleSystemSettings::ValueMode::kRandomWithin:
            return "Random Within";
    }
    return "#Invalid";
}

std::string InspectorPanel::GetEmissionShapeString(snowpulse::ParticleSystemSettings::EmissionShape emissionShape) {
    switch (emissionShape) {
        case snowpulse::ParticleSystemSettings::EmissionShape::kPoint:
            return "Point";
        case snowpulse::ParticleSystemSettings::EmissionShape::kRect:
            return "Rectangle";
        case snowpulse::ParticleSystemSettings::EmissionShape::kCircle:
            return "Circle";
    }
    return "#Invalid";
}

void InspectorPanel::DrawCombo(std::string label, int* value, std::vector<std::string> items) {
    ImGui::Text("%s", label.c_str());
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    const char* values[items.size()];
    int i = 0;
    for (const auto& item : items) {
        values[i++] = item.c_str();
    }
    ImGui::Combo((std::string("##") + label).c_str(), value, values, items.size());
}

void InspectorPanel::DrawInputFloat(std::string label, float* value, float increment, float fastIncrement) {
    ImGui::Text("%s", label.c_str());
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputFloat((std::string("##") + label).c_str(), value, increment, fastIncrement, "%.2f");
}

void InspectorPanel::DrawInputSize(std::string label, float* width, float* height, float increment, float fastIncrement) {
    ImGui::Text("%s", label.c_str());
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x / 2);
    ImGui::InputFloat((std::string("##") + label + "_width").c_str(), width, increment, fastIncrement, "%.2f");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputFloat((std::string("##") + label + "_height").c_str(), height, increment, fastIncrement, "%.2f");
}
}   // namespace snowpulse
