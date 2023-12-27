#include "effect_data.h"

namespace game {
EffectData::EffectData() {

}

EffectData::~EffectData() {

}

bool EffectData::LoadFromFile(std::string filename) {
    auto json = snowpulse::JsonUtils::LoadFile(filename, snowpulse::PathType::kRaw);
    if (!json) {
        return false;
    }

    settings_.acceleration.x = (*json)["acceleration_x"];
    settings_.acceleration.y = (*json)["acceleration_y"];
    settings_.angleStart = (*json)["angleStart"];
    settings_.angleVelocity = (*json)["angleVelocity"];
    settings_.blendMode = (snowpulse::BlendMode)((*json)["blendMode"]);
    settings_.colorEnd.r = (*json)["colorEnd_r"];
    settings_.colorEnd.g = (*json)["colorEnd_g"];
    settings_.colorEnd.b = (*json)["colorEnd_b"];
    settings_.colorEnd.a = (*json)["colorEnd_a"];
    settings_.colorStart.r = (*json)["colorStart_r"];
    settings_.colorStart.g = (*json)["colorStart_g"];
    settings_.colorStart.b = (*json)["colorStart_b"];
    settings_.colorStart.a =(*json)["colorStart_a"];
    settings_.emissionAngleA = (*json)["emissionAngleA"];
    settings_.emissionAngleB = (*json)["emissionAngleB"];
    settings_.emissionAngleValueMode = (snowpulse::ParticleSystemSettings::ValueMode)((*json)["emissionAngleValueMode"]);
    settings_.emissionRadius = (*json)["emissionRadius"];
    settings_.emissionRate = (*json)["emissionRate"];
    settings_.emissionRectSize.x = (*json)["emissionRectSize_x"];
    settings_.emissionRectSize.y = (*json)["emissionRectSize_y"];
    settings_.emissionShape = (snowpulse::ParticleSystemSettings::EmissionShape)((*json)["emissionShape"]);
    settings_.lifespanA = (*json)["lifespanA"];
    settings_.lifespanB = (*json)["lifespanB"];
    settings_.lifespanValueMode = (snowpulse::ParticleSystemSettings::ValueMode)((*json)["lifespanValueMode"]);
    settings_.maxParticleCount = (*json)["maxParticleCount"];
    settings_.scaleStartEnd.x = (*json)["scaleStartEnd_x"];
    settings_.scaleStartEnd.y = (*json)["scaleStartEnd_y"];
    settings_.speedA = (*json)["speedA"];
    settings_.speedB = (*json)["speedB"];
    settings_.speedValueMode = (snowpulse::ParticleSystemSettings::ValueMode)((*json)["speedValueMode"]);
    settings_.textureFilename = (*json)["textureFilename"];
    settings_.textureFiltering = (snowpulse::TextureFiltering)((*json)["textureFiltering"]);
    settings_.texturePathType = (snowpulse::PathType)((*json)["texturePathType"]);

    previousFilename_ = filename;
    return true;
}

void EffectData::SaveToFile(std::string filename) {
    auto json = snowpulse::JsonUtils::Create();
    (*json)["acceleration_x"] = settings_.acceleration.x;
    (*json)["acceleration_y"] = settings_.acceleration.y;
    (*json)["angleStart"] = settings_.angleStart;
    (*json)["angleVelocity"] = settings_.angleVelocity;
    (*json)["blendMode"] = (int)settings_.blendMode;
    (*json)["colorEnd_r"] = settings_.colorEnd.r;
    (*json)["colorEnd_g"] = settings_.colorEnd.g;
    (*json)["colorEnd_b"] = settings_.colorEnd.b;
    (*json)["colorEnd_a"] = settings_.colorEnd.a;
    (*json)["colorStart_r"] = settings_.colorStart.r;
    (*json)["colorStart_g"] = settings_.colorStart.g;
    (*json)["colorStart_b"] = settings_.colorStart.b;
    (*json)["colorStart_a"] = settings_.colorStart.a;
    (*json)["emissionAngleA"] = settings_.emissionAngleA;
    (*json)["emissionAngleB"] = settings_.emissionAngleB;
    (*json)["emissionAngleValueMode"] = (int)settings_.emissionAngleValueMode;
    (*json)["emissionRadius"] = settings_.emissionRadius;
    (*json)["emissionRate"] = settings_.emissionRate;
    (*json)["emissionRectSize_x"] = settings_.emissionRectSize.x;
    (*json)["emissionRectSize_y"] = settings_.emissionRectSize.y;
    (*json)["emissionShape"] = (int)settings_.emissionShape;
    (*json)["lifespanA"] = settings_.lifespanA;
    (*json)["lifespanB"] = settings_.lifespanB;
    (*json)["lifespanValueMode"] = (int)settings_.lifespanValueMode;
    (*json)["maxParticleCount"] = settings_.maxParticleCount;
    (*json)["scaleStartEnd_x"] = settings_.scaleStartEnd.x;
    (*json)["scaleStartEnd_y"] = settings_.scaleStartEnd.y;
    (*json)["speedA"] = settings_.speedA;
    (*json)["speedB"] = settings_.speedB;
    (*json)["speedValueMode"] = (int)settings_.speedValueMode;
    (*json)["textureFilename"] = settings_.textureFilename;
    (*json)["textureFiltering"] = (int)settings_.textureFiltering;
    (*json)["texturePathType"] = (int)settings_.texturePathType;

    snowpulse::JsonUtils::SaveFile(json.get(), filename, snowpulse::PathType::kRaw);
    previousFilename_ = filename;
}
bool EffectData::Save() {
    if (previousFilename_ == "") {
        return false;
    }
    SaveToFile(previousFilename_);
    return true;
}

void EffectData::Reset() {
    settings_ = snowpulse::ParticleSystemSettings();
    settings_.blendMode = snowpulse::BlendMode::kNormal;
    settings_.scaleStartEnd = snowpulse::Vector2(2.0f, 0.2f);
    settings_.lifespanValueMode = snowpulse::ParticleSystemSettings::ValueMode::kRandomWithin;
    settings_.lifespanA = 0.8f;
    settings_.lifespanB = 1.0f;
    settings_.maxParticleCount = 300;
    settings_.emissionShape = snowpulse::ParticleSystemSettings::EmissionShape::kCircle;
    settings_.emissionRectSize = snowpulse::Vector2(100.0f, 40.0f);
    settings_.emissionRadius = 20.0f;
    settings_.emissionRate = 40.0f;
    settings_.emissionAngleValueMode = snowpulse::ParticleSystemSettings::ValueMode::kRandomWithin;
    settings_.emissionAngleA = 80.0f;
    settings_.emissionAngleB = 100.0f;
    settings_.speedValueMode = snowpulse::ParticleSystemSettings::ValueMode::kRandomWithin;
    settings_.speedA = 270.0f;
    settings_.speedB = 350.0f;
    settings_.acceleration = snowpulse::Vector2(0.0f, -1.0f) * 200.0f;
    settings_.colorStart = snowpulse::Color::Red();
    settings_.colorEnd = snowpulse::Color(1.0f, 0.6f, 0.0f, 0.0f);

    previousFilename_ = "";
}
}
