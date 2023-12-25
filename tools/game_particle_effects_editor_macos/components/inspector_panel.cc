#include "inspector_panel.h"

#include <imgui.h>

namespace game {
std::shared_ptr<InspectorPanel> InspectorPanel::Create() {
    auto panel = std::shared_ptr<InspectorPanel>(new InspectorPanel());
    return panel;
}

InspectorPanel::InspectorPanel() : Component("inspector_panel") {
    blendModes_.reserve(5);
    blendModes_.push_back(GetBlendModeString(snowpulse::BlendMode::kDisabled));
    blendModes_.push_back(GetBlendModeString(snowpulse::BlendMode::kNormal));
    blendModes_.push_back(GetBlendModeString(snowpulse::BlendMode::kAdditive));
    blendModes_.push_back(GetBlendModeString(snowpulse::BlendMode::kMultiply));
    blendModes_.push_back(GetBlendModeString(snowpulse::BlendMode::kScreen));
    textureFilters_.reserve(4);
    textureFilters_.push_back(GetTextureFilteringString(snowpulse::TextureFiltering::kPoint));
    textureFilters_.push_back(GetTextureFilteringString(snowpulse::TextureFiltering::kBilinear));
    textureFilters_.push_back(GetTextureFilteringString(snowpulse::TextureFiltering::kTrilinear));
    textureFilters_.push_back(GetTextureFilteringString(snowpulse::TextureFiltering::kAnisotropic));
    valueModes_.reserve(3);
    valueModes_.push_back(GetValueModeString(snowpulse::ParticleSystemSettings::ValueMode::kSingle));
    valueModes_.push_back(GetValueModeString(snowpulse::ParticleSystemSettings::ValueMode::kRandomBetween));
    valueModes_.push_back(GetValueModeString(snowpulse::ParticleSystemSettings::ValueMode::kRandomWithin));
    emissionShapes_.reserve(3);
    emissionShapes_.push_back(GetEmissionShapeString(snowpulse::ParticleSystemSettings::EmissionShape::kPoint));
    emissionShapes_.push_back(GetEmissionShapeString(snowpulse::ParticleSystemSettings::EmissionShape::kRect));
    emissionShapes_.push_back(GetEmissionShapeString(snowpulse::ParticleSystemSettings::EmissionShape::kCircle));

    blendMode_ = 1;
    textureFiltering_ = 1;

    ImGuiIO& io = ImGui::GetIO();
    auto fontFilename = snowpulse::Directory::GetInstance()->GetDefaultsPath("fonts/roboto/Roboto-Light.ttf");
    imguiFontNormal_ = io.Fonts->AddFontFromFileTTF(fontFilename.c_str(), 16.0f);
    if (!imguiFontNormal_) {
        io.Fonts->Build();
    }
    fontFilename = snowpulse::Directory::GetInstance()->GetDefaultsPath("fonts/roboto/Roboto-Bold.ttf");
    imguiFontH1_ = io.Fonts->AddFontFromFileTTF(fontFilename.c_str(), 18.0f);
    if (!imguiFontH1_) {
        io.Fonts->Build();
    }
    fontFilename = snowpulse::Directory::GetInstance()->GetDefaultsPath("fonts/roboto/Roboto-Bold.ttf");
    imguiFontH2_ = io.Fonts->AddFontFromFileTTF(fontFilename.c_str(), 16.0f);
    if (!imguiFontH2_) {
        io.Fonts->Build();
    }
    fontFilename = snowpulse::Directory::GetInstance()->GetDefaultsPath("fonts/roboto/Roboto-Regular.ttf");
    imguiFontH3_ = io.Fonts->AddFontFromFileTTF(fontFilename.c_str(), 16.0f);
    if (!imguiFontH3_) {
        io.Fonts->Build();
    }
}

InspectorPanel::~InspectorPanel() {
    
}

void InspectorPanel::Update(float deltaTime) {
    settings_.emissionShape = (snowpulse::ParticleSystemSettings::EmissionShape)emissionShape_;
    settings_.lifespanValueMode = (snowpulse::ParticleSystemSettings::ValueMode)lifespanValueMode_;
    settings_.blendMode = (snowpulse::BlendMode)blendMode_;
    settings_.textureFiltering = (snowpulse::TextureFiltering)textureFiltering_;
}

void InspectorPanel::Draw(snowpulse::Graphics* graphics, snowpulse::Matrix4x4 worldMatrix) {
    auto windowHeight = snowpulse::Application::GetInstance()->GetScreenSize().y;
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(350, windowHeight));
    ImGui::PushFont(imguiFontNormal_);
    ImGui::Begin("Inspector", SPNULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    DrawCombo("Emission Shape", &emissionShape_, emissionShapes_);
    if (settings_.emissionShape == snowpulse::ParticleSystemSettings::EmissionShape::kPoint) {
    }
    else if (settings_.emissionShape == snowpulse::ParticleSystemSettings::EmissionShape::kRect) {
        DrawInputSize("Emission Rect Size", &settings_.emissionRectSize.x, &settings_.emissionRectSize.y);
    }
    else if (settings_.emissionShape == snowpulse::ParticleSystemSettings::EmissionShape::kCircle) {
        DrawInputFloat("Emission Radius", &settings_.emissionRadius);
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

    DrawCombo("Blend Mode", &blendMode_, blendModes_);
    DrawCombo("Texture Filtering", &textureFiltering_, textureFilters_);

    ImGui::End();
    ImGui::PopFont();
}

std::string InspectorPanel::GetBlendModeString(snowpulse::BlendMode blendMode) {
    switch (blendMode) {
        case snowpulse::BlendMode::kNormal:
            return "Normal";
        case snowpulse::BlendMode::kAdditive:
            return "Additive";
        case snowpulse::BlendMode::kMultiply:
            return "Multiply";
        case snowpulse::BlendMode::kScreen:
            return "Screen";
        case snowpulse::BlendMode::kDisabled:
            return "Disabled";
    }
    return "#Invalid";
}

std::string InspectorPanel::GetTextureFilteringString(snowpulse::TextureFiltering textureFiltering) {
    switch (textureFiltering) {
        case snowpulse::TextureFiltering::kPoint:
            return "Point";
        case snowpulse::TextureFiltering::kBilinear:
            return "Bilinear";
        case snowpulse::TextureFiltering::kTrilinear:
            return "Trilinear";
        case snowpulse::TextureFiltering::kAnisotropic:
            return "Anisotropic";
    }
    return "#Invalid";
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
    ImGui::PushFont(imguiFontH3_);
    ImGui::Text("%s", label.c_str());
    ImGui::PopFont();
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    const char* values[items.size()];
    int i = 0;
    for (const auto& item : items) {
        values[i++] = item.c_str();
    }
    ImGui::Combo((std::string("##") + label).c_str(), value, values, (int)items.size());
}

void InspectorPanel::DrawInputFloat(std::string label, float* value, float increment, float fastIncrement) {
    ImGui::PushFont(imguiFontH3_);
    ImGui::Text("%s", label.c_str());
    ImGui::PopFont();
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputFloat((std::string("##") + label).c_str(), value, increment, fastIncrement, "%.2f");
}

void InspectorPanel::DrawInputSize(std::string label, float* width, float* height, float increment, float fastIncrement) {
    ImGui::PushFont(imguiFontH3_);
    ImGui::Text("%s", label.c_str());
    ImGui::PopFont();
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x / 2);
    ImGui::InputFloat((std::string("##") + label + "_width").c_str(), width, increment, fastIncrement, "%.2f");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputFloat((std::string("##") + label + "_height").c_str(), height, increment, fastIncrement, "%.2f");
}
}   // namespace snowpulse
