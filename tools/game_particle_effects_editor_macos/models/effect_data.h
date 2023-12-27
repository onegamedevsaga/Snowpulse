#ifndef GAME_MODELS_EFFECTDATA_H_
#define GAME_MODELS_EFFECTDATA_H_

#include "../defines.h"
#include <snowpulse.h>

#include <string>

namespace game {
class GAMEAPI EffectData {
    public:
        EffectData();
        virtual ~EffectData();
        bool LoadFromFile(std::string filename);
        void SaveToFile(std::string filename);
        bool Save();
        void Reset();
        void CopySettings(snowpulse::ParticleSystemSettings settings) { settings_ = settings; }

        snowpulse::ParticleSystemSettings& GetSettings() { return settings_; }

    private:
        std::string previousFilename_;
        snowpulse::ParticleSystemSettings settings_;
        snowpulse::Json jsonData_;
};
}
#endif
