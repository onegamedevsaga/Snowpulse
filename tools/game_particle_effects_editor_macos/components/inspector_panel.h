#ifndef GAME_COMPONENTS_INSPECTORPANEL_H_
#define GAME_COMPONENTS_INSPECTORPANEL_H_

#include "../defines.h"

#include <memory>
#include <functional>
#include <snowpulse.h>

namespace game {
class GAMEAPI InspectorPanel : public snowpulse::Component, public snowpulse::Drawable {
    public:
        static std::shared_ptr<InspectorPanel> Create();

        virtual ~InspectorPanel();
        // From Updatable
        void Update(float deltaTime) override;
        // From Drawable
        void Draw(snowpulse::Graphics* graphics, snowpulse::Matrix4x4 worldMatrix) override;

        void SetListener(std::function<void(snowpulse::ParticleSystemSettings)> onInvalidate) { onInvalidate_ = onInvalidate; }
        void Invalidate();

    private:
        InspectorPanel();

        std::string GetBlendModeString(snowpulse::BlendMode blendMode);
        std::string GetTextureFilteringString(snowpulse::TextureFiltering textureFiltering);
        std::string GetPathTypeString(snowpulse::PathType pathType);
        std::string GetValueModeString(snowpulse::ParticleSystemSettings::ValueMode valueMode);
        std::string GetEmissionShapeString(snowpulse::ParticleSystemSettings::EmissionShape emissionShape);
        void DrawHeader(std::string label);
        bool DrawCombo(std::string label, int* value, std::vector<std::string> items);
        bool DrawInputInt(std::string label, int* value, int increment = 1, int fastIncrement = 5);
        bool DrawInputFloat(std::string label, float* value, float increment = 0.1f, float fastIncrement = 1.0f);
        bool DrawInputVec2(std::string label, float* x, float* y, float increment = 0.1f, float fastIncrement = 1.0f);
        bool DrawInputText(std::string label, char* value);
        bool DrawInputBrowseFile(std::string label, char* value);
        bool DrawInputColor(std::string label, float* value);
        bool DrawInputColor2(std::string label, float* valueA, float* valueB);

        std::function<void(snowpulse::ParticleSystemSettings)> onInvalidate_;
        ImFont* imguiFontH1_;
        ImFont* imguiFontH2_;
        ImFont* imguiFontH3_;
        ImFont* imguiFontNormal_;
        std::vector<std::string> blendModes_;
        std::vector<std::string> textureFilters_;
        std::vector<std::string> pathTypes_;
        std::vector<std::string> valueModes_;
        std::vector<std::string> emissionShapes_;
        snowpulse::ParticleSystemSettings settings_;
        int blendMode_;
        int textureFiltering_;
        int pathType_;
        char textureFilename_[256];
        int lifespanValueMode_;
        int speedValueMode_;
        float colorStart_[4];
        float colorEnd_[4];
        int emissionShape_;
        int emissionAngleValueMode_;
};
}   // namespace snowpulse
#endif
