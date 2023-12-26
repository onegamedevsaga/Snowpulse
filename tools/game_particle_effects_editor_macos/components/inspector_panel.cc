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
    pathTypes_.reserve(5);
    pathTypes_.push_back(GetPathTypeString(snowpulse::PathType::kAssets));
    pathTypes_.push_back(GetPathTypeString(snowpulse::PathType::kDefaults));
    pathTypes_.push_back(GetPathTypeString(snowpulse::PathType::kApplicationSupport));
    pathTypes_.push_back(GetPathTypeString(snowpulse::PathType::kDocuments));
    pathTypes_.push_back(GetPathTypeString(snowpulse::PathType::kRaw));
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
    pathType_ = 0;

    for (int i = 0; i < 4; i++) {
        colorStart_[i] = 1.0f;
        colorEnd_[i] = 1.0f;
    }

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
}

void InspectorPanel::Draw(snowpulse::Graphics* graphics, snowpulse::Matrix4x4 worldMatrix) {
    auto windowSize = snowpulse::Application::GetInstance()->GetScreenSize();
    auto actionPanelHeight = 40.0f;
    auto panelWidth = 375.0f;
    ImGui::SetNextWindowPos(ImVec2(windowSize.x - panelWidth, actionPanelHeight));
    ImGui::SetNextWindowSize(ImVec2(panelWidth, windowSize.y - actionPanelHeight));
    ImGui::PushFont(imguiFontNormal_);
    ImGui::Begin(" Inspector", SPNULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    if (DrawInputInt("Max Particle Count", &settings_.maxParticleCount)) {
        settings_.maxParticleCount = settings_.maxParticleCount < 0 ? 0 : settings_.maxParticleCount;
    }

    if (DrawCombo("Emission Shape", &emissionShape_, emissionShapes_)) {
        settings_.emissionShape = (snowpulse::ParticleSystemSettings::EmissionShape)emissionShape_;
    }
    if (settings_.emissionShape == snowpulse::ParticleSystemSettings::EmissionShape::kPoint) {
    }
    else if (settings_.emissionShape == snowpulse::ParticleSystemSettings::EmissionShape::kRect) {
        DrawInputVec2("Emission Rect Size", &settings_.emissionRectSize.x, &settings_.emissionRectSize.y);
    }
    else if (settings_.emissionShape == snowpulse::ParticleSystemSettings::EmissionShape::kCircle) {
        DrawInputFloat("Emission Radius", &settings_.emissionRadius);
    }

    if (DrawCombo("Emission Angle Value Mode", &emissionAngleValueMode_, valueModes_)) {
        settings_.emissionAngleValueMode = (snowpulse::ParticleSystemSettings::ValueMode)emissionAngleValueMode_;
    }
    if (settings_.emissionAngleValueMode == snowpulse::ParticleSystemSettings::ValueMode::kSingle) {
        DrawInputFloat("Emission Angle", &settings_.emissionAngleA);
    }
    else if (settings_.emissionAngleValueMode == snowpulse::ParticleSystemSettings::ValueMode::kRandomBetween) {
        DrawInputFloat("Emission Angle A", &settings_.emissionAngleA);
        DrawInputFloat("Emission Angle B", &settings_.emissionAngleB);
    }
    else if (settings_.emissionAngleValueMode == snowpulse::ParticleSystemSettings::ValueMode::kRandomWithin) {
        DrawInputFloat("Emission Angle From", &settings_.emissionAngleA);
        DrawInputFloat("Emission Angle To", &settings_.emissionAngleB);
    }

    if (DrawCombo("Lifespan Value Mode", &lifespanValueMode_, valueModes_)) {
        settings_.lifespanValueMode = (snowpulse::ParticleSystemSettings::ValueMode)lifespanValueMode_;
    }
    if (settings_.lifespanValueMode == snowpulse::ParticleSystemSettings::ValueMode::kSingle) {
        if (DrawInputFloat("Lifespan", &settings_.lifespanA)) {
            settings_.lifespanA = settings_.lifespanA < 0.0f ? 0.0f : settings_.lifespanA;
        }
    }
    else if (settings_.lifespanValueMode == snowpulse::ParticleSystemSettings::ValueMode::kRandomBetween) {
        if (DrawInputFloat("Lifespan A", &settings_.lifespanA)) {
            settings_.lifespanA = settings_.lifespanA < 0.0f ? 0.0f : settings_.lifespanA;
        }
        if (DrawInputFloat("Lifespan B", &settings_.lifespanB)) {
            settings_.lifespanB = settings_.lifespanB < 0.0f ? 0.0f : settings_.lifespanB;
        }
    }
    else if (settings_.lifespanValueMode == snowpulse::ParticleSystemSettings::ValueMode::kRandomWithin) {
        if (DrawInputFloat("Lifespan From", &settings_.lifespanA)) {
            settings_.lifespanA = settings_.lifespanA < 0.0f ? 0.0f : settings_.lifespanA;
        }
        if (DrawInputFloat("Lifespan To", &settings_.lifespanB)) {
            settings_.lifespanB = settings_.lifespanB < 0.0f ? 0.0f : settings_.lifespanB;
        }
    }

    if (DrawCombo("Speed Value Mode", &speedValueMode_, valueModes_)) {
        settings_.speedValueMode = (snowpulse::ParticleSystemSettings::ValueMode)speedValueMode_;
    }
    if (settings_.speedValueMode == snowpulse::ParticleSystemSettings::ValueMode::kSingle) {
        if (DrawInputFloat("Speed", &settings_.speedA)) {
            settings_.speedA = settings_.speedA < 0.0f ? 0.0f : settings_.speedA;
        }
    }
    else if (settings_.speedValueMode == snowpulse::ParticleSystemSettings::ValueMode::kRandomBetween) {
        if (DrawInputFloat("Speed A", &settings_.speedA)) {
            settings_.speedA = settings_.speedA < 0.0f ? 0.0f : settings_.speedA;
        }
        if (DrawInputFloat("Speed B", &settings_.speedB)) {
            settings_.speedB = settings_.speedB < 0.0f ? 0.0f : settings_.speedB;
        }
    }
    else if (settings_.speedValueMode == snowpulse::ParticleSystemSettings::ValueMode::kRandomWithin) {
        if (DrawInputFloat("Speed From", &settings_.speedA)) {
            settings_.speedA = settings_.speedA < 0.0f ? 0.0f : settings_.speedA;
        }
        if (DrawInputFloat("Speed To", &settings_.speedB)) {
            settings_.speedB = settings_.speedB < 0.0f ? 0.0f : settings_.speedB;
        }
    }

    if (DrawInputColor2("Color Start/End", colorStart_, colorEnd_)) {
        settings_.colorStart.r = colorStart_[0];
        settings_.colorStart.g = colorStart_[1];
        settings_.colorStart.b = colorStart_[2];
        settings_.colorStart.a = colorStart_[3];
        settings_.colorEnd.r = colorStart_[0];
        settings_.colorEnd.g = colorStart_[1];
        settings_.colorEnd.b = colorStart_[2];
        settings_.colorEnd.a = colorStart_[3];
    }

    DrawInputVec2("Acceleration", &settings_.acceleration.x, &settings_.acceleration.y);
    DrawInputVec2("Scale Start/End", &settings_.scaleStartEnd.x, &settings_.scaleStartEnd.y);
    DrawInputFloat("Angle(Deg) Start", &settings_.angleStart);
    DrawInputFloat("Angle(Deg) Velocity", &settings_.angleVelocity);

    DrawHeader("Texture");
    if (DrawInputBrowseFile("Texture Filename", textureFilename_)) {
        // TODO: Update preview's particle
        settings_.textureFilename = std::string(textureFilename_);
    }
    if (DrawCombo("Blend Mode", &blendMode_, blendModes_)) {
        settings_.blendMode = (snowpulse::BlendMode)blendMode_;
    }
    if (DrawCombo("Texture Filtering", &textureFiltering_, textureFilters_)) {
        settings_.textureFiltering = (snowpulse::TextureFiltering)textureFiltering_;
    }

    DrawHeader("Runtime Loading");
    if (DrawCombo("Path Type", &pathType_, pathTypes_)) {
        settings_.texturePathType = (snowpulse::PathType)pathType_;
    }

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

std::string InspectorPanel::GetPathTypeString(snowpulse::PathType pathType) {
    switch (pathType) {
        case snowpulse::PathType::kAssets:
            return "Assets";
        case snowpulse::PathType::kDefaults:
            return "Defaults";
        case snowpulse::PathType::kApplicationSupport:
            return "Application Support";
        case snowpulse::PathType::kDocuments:
            return "Documents";
        case snowpulse::PathType::kRaw:
            return "Raw";
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

void InspectorPanel::DrawHeader(std::string label) {
    ImGui::Dummy(ImVec2(0.0f, 5.0f));
    ImGui::PushFont(imguiFontH1_);
    ImGui::Text("%s", label.c_str());
    ImGui::PopFont();
}

bool InspectorPanel::DrawCombo(std::string label, int* value, std::vector<std::string> items) {
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
    return ImGui::Combo((std::string("##") + label).c_str(), value, values, (int)items.size());
}

bool InspectorPanel::DrawInputInt(std::string label, int* value, int increment, int fastIncrement) {
    ImGui::PushFont(imguiFontH3_);
    ImGui::Text("%s", label.c_str());
    ImGui::PopFont();
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    return ImGui::InputInt((std::string("##") + label).c_str(), value, increment, fastIncrement);
}

bool InspectorPanel::DrawInputFloat(std::string label, float* value, float increment, float fastIncrement) {
    ImGui::PushFont(imguiFontH3_);
    ImGui::Text("%s", label.c_str());
    ImGui::PopFont();
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    return ImGui::InputFloat((std::string("##") + label).c_str(), value, increment, fastIncrement, "%.2f");
}

bool InspectorPanel::DrawInputVec2(std::string label, float* x, float* y, float increment, float fastIncrement) {
    ImGui::PushFont(imguiFontH3_);
    ImGui::Text("%s", label.c_str());
    ImGui::PopFont();
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x / 2);
    bool modified = ImGui::InputFloat((std::string("##") + label + "_x").c_str(), x, increment, fastIncrement, "%.2f");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    modified = ImGui::InputFloat((std::string("##") + label + "_y").c_str(), y, increment, fastIncrement, "%.2f") || modified;
    return modified;
}

bool InspectorPanel::DrawInputText(std::string label, char* value) {
    ImGui::PushFont(imguiFontH3_);
    ImGui::Text("%s", label.c_str());
    ImGui::PopFont();
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    return ImGui::InputText((std::string("##") + label).c_str(), value, 256);
}

bool InspectorPanel::DrawInputBrowseFile(std::string label, char* value) {
    ImGui::PushFont(imguiFontH3_);
    ImGui::Text("%s", label.c_str());
    ImGui::PopFont();
    ImGui::SameLine();
    ImGui::SetNextItemWidth((ImGui::GetContentRegionAvail().x / 3.0f) * 2.0f);
    bool modified = ImGui::InputText((std::string("##") + label).c_str(), value, 256);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    if (ImGui::Button("Browse", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
        char const* filterPatterns[4] = { "*.jpg", "*.png", "*.bmp" };
        const char* selectedPath = tinyfd_openFileDialog(
            "Select File",    // Dialog title
            "",               // Default path
            3,                // Number of filter patterns
            filterPatterns,   // Filter patterns
            "Image Files",             // Single filter description
            0                 // Allow multiple selections
        );

        if (selectedPath) {
            value = std::strcpy(value, selectedPath);
        }
        modified = true;
    }
    
    return modified;
}

bool InspectorPanel::DrawInputColor(std::string label, float* value) {
    ImGui::PushFont(imguiFontH3_);
    ImGui::Text("%s", label.c_str());
    ImGui::PopFont();
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    return ImGui::ColorEdit4((std::string("##") + label).c_str(), value, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar);
}

bool InspectorPanel::DrawInputColor2(std::string label, float* valueA, float* valueB) {
    ImGui::PushFont(imguiFontH3_);
    ImGui::Text("%s", label.c_str());
    ImGui::PopFont();
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x / 2);
    bool modified = ImGui::ColorEdit4((std::string("##") + label + "_a").c_str(), valueA, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    modified = modified || ImGui::ColorEdit4((std::string("##") + label + "_b").c_str(), valueB, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_AlphaBar);
    return modified;
}
}   // namespace snowpulse
