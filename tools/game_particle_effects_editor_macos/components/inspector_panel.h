#ifndef GAME_COMPONENTS_INSPECTORPANEL_H_
#define GAME_COMPONENTS_INSPECTORPANEL_H_

#include "../defines.h"

#include <memory>
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

    protected:
        InspectorPanel();
        std::string GetValueModeString(snowpulse::ParticleSystemSettings::ValueMode valueMode);
        std::string GetEmissionShapeString(snowpulse::ParticleSystemSettings::EmissionShape emissionShape);
        void DrawCombo(std::string label, int* value, std::vector<std::string> items);
        void DrawInputFloat(std::string label, float* value, float increment = 0.1f, float fastIncrement = 1.0f);
        void DrawInputSize(std::string label, float* width, float* height, float increment = 0.1f, float fastIncrement = 1.0f);

        std::vector<std::string> valueModes_;
        std::vector<std::string> emissionShapes_;
        snowpulse::ParticleSystemSettings settings_;
        int lifespanValueMode_;
        int speedValueMode_;
        int emissionShape_;
        int emissionAngleValueMode_;
};
}   // namespace snowpulse
#endif
