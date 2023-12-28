#include "action_panel.h"

#include <imgui.h>

namespace game {
std::shared_ptr<ActionPanel> ActionPanel::Create() {
    auto panel = std::shared_ptr<ActionPanel>(new ActionPanel());
    return panel;
}

ActionPanel::ActionPanel() : Component("action_panel"), onLoadFile_(SPNULL), openLoadingInvalidFilePopup_(false), isFirstTimeDrawingMenu_(true) {
}

ActionPanel::~ActionPanel() {
}

void ActionPanel::Update(float deltaTime) {
    auto input = snowpulse::Input::GetInstance();

#ifdef SNOWPULSE_PLATFORM_MACOS
    if (input->GetDown("super") && input->GetPressed("n")) {
#elif SNOWPULSE_PLATFORM_WINDOWS
    if (input->GetDown("control") && input->GetPressed("n")) {
#endif
        CreateNewEffect();
    }

#ifdef SNOWPULSE_PLATFORM_MACOS
    if (input->GetDown("super") && input->GetPressed("o")) {
#elif SNOWPULSE_PLATFORM_WINDOWS
    if (input->GetDown("control") && input->GetPressed("o")) {
#endif
        LoadEffect();
    }

#ifdef SNOWPULSE_PLATFORM_MACOS
    if (input->GetDown("super") && input->GetPressed("s")) {
#elif SNOWPULSE_PLATFORM_WINDOWS
    if (input->GetDown("control") && input->GetPressed("s")) {
#endif
        SaveEffect();
    }

#ifdef SNOWPULSE_PLATFORM_MACOS
    if (input->GetDown("super") && input->GetDown("shift") && input->GetPressed("s")) {
#elif SNOWPULSE_PLATFORM_WINDOWS
    if (input->GetDown("control") && input->GetDown("shift") && input->GetPressed("s")) {
#endif
        SaveAsEffect();
    }

#ifdef SNOWPULSE_PLATFORM_MACOS
    if (input->GetDown("super") && input->GetPressed("r")) {
#elif SNOWPULSE_PLATFORM_WINDOWS
    if (input->GetDown("control") && input->GetPressed("r")) {
#endif
        ResetView();
    }

#ifdef SNOWPULSE_PLATFORM_MACOS
    if (input->GetDown("super") && input->GetPressed("x")) {
#elif SNOWPULSE_PLATFORM_WINDOWS
    if (input->GetDown("control") && input->GetPressed("x")) {
#endif
        snowpulse::Application::GetInstance()->Close();
    }
}

void ActionPanel::Draw(snowpulse::Graphics* graphics, snowpulse::Matrix4x4 worldMatrix) {
    auto windowSize = snowpulse::Application::GetInstance()->GetScreenSize();
    auto panelWidth = windowSize.x;
    ImGuiStyle& style = ImGui::GetStyle();
    auto prevWindowRounding = style.WindowRounding;
    style.WindowRounding = 0.0f;

    DrawMenu();
    float menuBarHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2;

    ImGui::SetNextWindowPos(ImVec2(0.0f, menuBarHeight));
    ImGui::SetNextWindowSize(ImVec2(panelWidth, 40.0f));
    ImGui::Begin("Actions", SPNULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    ImGui::End();
    style.WindowRounding = prevWindowRounding;

    if (openLoadingInvalidFilePopup_) {
        ImGui::OpenPopup(" Loading Invalid File");
        openLoadingInvalidFilePopup_ = false;
    }

    if (ImGui::BeginPopupModal(" Loading Invalid File", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Dummy(ImVec2(270.0f, 20.0f));
        ImGui::Text("You must load an SPPE (.sppe) file.");
        ImGui::Dummy(ImVec2(0.0f, 20.0f));

        auto width = ImGui::GetContentRegionAvail().x * 0.5f;
        ImGui::SetNextItemWidth(width);
        if (ImGui::Button("Cancel", ImVec2(width, 40.0f))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        width = ImGui::GetContentRegionAvail().x;
        ImGui::SetNextItemWidth(width);
        if (ImGui::Button("Try Again", ImVec2(width, 40.0f))) {
            ImGui::CloseCurrentPopup();
            LoadEffect();
        }

        ImGui::EndPopup();
    }
}

void ActionPanel::SetOnLoadListener(std::function<void(snowpulse::ParticleSystemSettings&)> onLoadFile) {
    onLoadFile_ = onLoadFile;
}

void ActionPanel::CreateNewEffect() {
    previousFilename_ = "";
    settings_ = snowpulse::ParticleSystemSettings();
    onLoadFile_(settings_);
    ResetView();
}

void ActionPanel::LoadEffect() {
#ifdef SNOWPULSE_PLATFORM_MACOS
    const char* filterPatterns[1] = {"**public.data"};
#else
    const char* filterPatterns[1] = {"*.sppe"};
#endif
    const char* loadPath = tinyfd_openFileDialog(
        "Load Effects File (.sppe)",
        "",
        1,
        filterPatterns,
        SPNULL,
        0
    );

    if (loadPath) {
        auto extension = snowpulse::Directory::GetInstance()->GetExtension(loadPath);
        if (extension == "sppe") {
            if (settings_.LoadFromSPPEFile(loadPath) && onLoadFile_) {
                onLoadFile_(settings_);
                previousFilename_ = loadPath;
                ResetView();
            }
        }
        else {
            openLoadingInvalidFilePopup_ = true;
        }
    }
}

void ActionPanel::SaveEffect() {
    if (previousFilename_ == "" || !settings_.SaveToSPPEFile(previousFilename_)) {
        SaveAsEffect();
    }
}
void ActionPanel::SaveAsEffect() {
    const char* filterPatterns[1] = { "*.sppe" };
    const char* defaultFileName = "untitled_particle_effect.sppe";
    const char* savePath = tinyfd_saveFileDialog(
        "Save Effects File",
        defaultFileName,
        1,
        filterPatterns,
        SPNULL
    );

    if (savePath) {
        settings_.SaveToSPPEFile(savePath);
        previousFilename_ = savePath;
    }
}

void ActionPanel::ResetView() {
    float menuBarHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2;
    auto actionPanelHeight = menuBarHeight + 40.0f;
    auto inspectorPanelWidth = 375.0f;
    auto camera = snowpulse::Application::GetInstance()->GetGraphics()->GetCamera();
    auto resolutionSize = snowpulse::Application::GetInstance()->GetResolutionSize();
    auto screenSize = snowpulse::Application::GetInstance()->GetScreenSize();
    auto screenToResolutionFactor = snowpulse::Vector2((float)resolutionSize.x / (float)screenSize.x, (float)resolutionSize.y / (float)screenSize.y);
    auto defaultCameraPosition = snowpulse::Vector2(inspectorPanelWidth, actionPanelHeight) * screenToResolutionFactor * 0.5f;
    camera->SetSize(1.0f);
    camera->SetPosition(defaultCameraPosition);
}

void ActionPanel::ZoomIn() {
    float menuBarHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2;
    auto actionPanelHeight = menuBarHeight + 40.0f;
    auto inspectorPanelWidth = 375.0f;
    auto camera = snowpulse::Application::GetInstance()->GetGraphics()->GetCamera();
    auto cameraSize = camera->GetSize();
    auto newCameraSize = cameraSize * 0.8f;
    auto sizeDelta = cameraSize - newCameraSize;
    auto cameraPos = camera->GetPosition();
    camera->SetSize(newCameraSize);
    camera->SetPosition(snowpulse::Vector2(cameraPos.x - inspectorPanelWidth * sizeDelta, cameraPos.y - actionPanelHeight * sizeDelta));
}

void ActionPanel::ZoomOut() {
    float menuBarHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2;
    auto actionPanelHeight = menuBarHeight + 40.0f;
    auto inspectorPanelWidth = 375.0f;
    auto camera = snowpulse::Application::GetInstance()->GetGraphics()->GetCamera();
    auto cameraSize = camera->GetSize();
    auto newCameraSize = cameraSize * 1.2f;
    auto sizeDelta = cameraSize - newCameraSize;
    auto cameraPos = camera->GetPosition();
    camera->SetSize(newCameraSize);
    camera->SetPosition(snowpulse::Vector2(cameraPos.x - inspectorPanelWidth * sizeDelta, cameraPos.y - actionPanelHeight * sizeDelta));
}

void ActionPanel::DrawMenu() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Create New", "Command/Ctrl + N")) {
                CreateNewEffect();
            }
            if (ImGui::MenuItem("Open", "Command/Ctrl + O")) {
                LoadEffect();
            }
            if (ImGui::MenuItem("Save", "Command/Ctrl + S")) {
                SaveEffect();
            }
            if (ImGui::MenuItem("Save As", "Command/Ctrl + Shift + S")) {
                SaveAsEffect();
            }
            if (ImGui::MenuItem("Exit", "Command/Ctrl + X")) {
                snowpulse::Application::GetInstance()->Close();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Reset", "Command/Ctrl + R")) {
                ResetView();
            }
            if (ImGui::MenuItem("Zoom In", "[")) {
                ZoomIn();
            }
            if (ImGui::MenuItem("Zoom Out", "]")) {
                ZoomOut();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();

        if (isFirstTimeDrawingMenu_) {
            ResetView();
            isFirstTimeDrawingMenu_ = false;
        }
    }
}
}   // namespace snowpulse
