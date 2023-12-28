#include "particle_system_settings.h"

#include <cctype>

#include "../../../common/json_utils.h"
#include "../../../common/string_utils.h"
#include "../../../application/directory.h"

namespace snowpulse {
bool ParticleSystemSettings::LoadFromSPPEFile(std::string filename) {
    if (StringUtils::ToLower(Directory::GetInstance()->GetExtension(filename)) != "sppe") {
        return false;
    }
    auto json = snowpulse::JsonUtils::LoadFile(filename, snowpulse::PathType::kRaw);
    if (!json) {
        return false;
    }

    acceleration.x = (*json)["acceleration_x"];
    acceleration.y = (*json)["acceleration_y"];
    angleStart = (*json)["angleStart"];
    angleVelocity = (*json)["angleVelocity"];
    blendMode = (snowpulse::BlendMode)((*json)["blendMode"]);
    colorEnd.r = (*json)["colorEnd_r"];
    colorEnd.g = (*json)["colorEnd_g"];
    colorEnd.b = (*json)["colorEnd_b"];
    colorEnd.a = (*json)["colorEnd_a"];
    colorStart.r = (*json)["colorStart_r"];
    colorStart.g = (*json)["colorStart_g"];
    colorStart.b = (*json)["colorStart_b"];
    colorStart.a =(*json)["colorStart_a"];
    emissionAngleA = (*json)["emissionAngleA"];
    emissionAngleB = (*json)["emissionAngleB"];
    emissionAngleValueMode = (snowpulse::ParticleSystemSettings::ValueMode)((*json)["emissionAngleValueMode"]);
    emissionRadius = (*json)["emissionRadius"];
    emissionRate = (*json)["emissionRate"];
    emissionRectSize.x = (*json)["emissionRectSize_x"];
    emissionRectSize.y = (*json)["emissionRectSize_y"];
    emissionShape = (snowpulse::ParticleSystemSettings::EmissionShape)((*json)["emissionShape"]);
    lifespanA = (*json)["lifespanA"];
    lifespanB = (*json)["lifespanB"];
    lifespanValueMode = (snowpulse::ParticleSystemSettings::ValueMode)((*json)["lifespanValueMode"]);
    maxParticleCount = (*json)["maxParticleCount"];
    scaleStartEnd.x = (*json)["scaleStartEnd_x"];
    scaleStartEnd.y = (*json)["scaleStartEnd_y"];
    speedA = (*json)["speedA"];
    speedB = (*json)["speedB"];
    speedValueMode = (snowpulse::ParticleSystemSettings::ValueMode)((*json)["speedValueMode"]);
    textureFilename = (*json)["textureFilename"];
    textureFiltering = (snowpulse::TextureFiltering)((*json)["textureFiltering"]);
    texturePathType = (snowpulse::PathType)((*json)["texturePathType"]);

    return true;
}

bool ParticleSystemSettings::SaveToSPPEFile(std::string filename) {
    auto json = snowpulse::JsonUtils::Create();
    (*json)["acceleration_x"] = acceleration.x;
    (*json)["acceleration_y"] = acceleration.y;
    (*json)["angleStart"] = angleStart;
    (*json)["angleVelocity"] = angleVelocity;
    (*json)["blendMode"] = (int)blendMode;
    (*json)["colorEnd_r"] = colorEnd.r;
    (*json)["colorEnd_g"] = colorEnd.g;
    (*json)["colorEnd_b"] = colorEnd.b;
    (*json)["colorEnd_a"] = colorEnd.a;
    (*json)["colorStart_r"] = colorStart.r;
    (*json)["colorStart_g"] = colorStart.g;
    (*json)["colorStart_b"] = colorStart.b;
    (*json)["colorStart_a"] = colorStart.a;
    (*json)["emissionAngleA"] = emissionAngleA;
    (*json)["emissionAngleB"] = emissionAngleB;
    (*json)["emissionAngleValueMode"] = (int)emissionAngleValueMode;
    (*json)["emissionRadius"] = emissionRadius;
    (*json)["emissionRate"] = emissionRate;
    (*json)["emissionRectSize_x"] = emissionRectSize.x;
    (*json)["emissionRectSize_y"] = emissionRectSize.y;
    (*json)["emissionShape"] = (int)emissionShape;
    (*json)["lifespanA"] = lifespanA;
    (*json)["lifespanB"] = lifespanB;
    (*json)["lifespanValueMode"] = (int)lifespanValueMode;
    (*json)["maxParticleCount"] = maxParticleCount;
    (*json)["scaleStartEnd_x"] = scaleStartEnd.x;
    (*json)["scaleStartEnd_y"] = scaleStartEnd.y;
    (*json)["speedA"] = speedA;
    (*json)["speedB"] = speedB;
    (*json)["speedValueMode"] = (int)speedValueMode;
    (*json)["textureFilename"] = textureFilename;
    (*json)["textureFiltering"] = (int)textureFiltering;
    (*json)["texturePathType"] = (int)texturePathType;

    return snowpulse::JsonUtils::SaveFile(json.get(), filename, snowpulse::PathType::kRaw);
}
}   // namespace snowpulse
