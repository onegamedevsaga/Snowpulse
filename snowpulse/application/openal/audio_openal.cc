#include "audio_openal.h"

#include <OpenAL/OpenAL.h>
#include <OpenAL/alc.h>


namespace snowpulse {

std::shared_ptr<AudioOpenAL> AudioOpenAL::Create() {
    auto audio = new AudioOpenAL();
    return std::shared_ptr<AudioOpenAL>(audio);
}

AudioOpenAL::AudioOpenAL() : Audio() {
}

AudioOpenAL::~AudioOpenAL() {
}

bool AudioOpenAL::Initialize() {
    ALCdevice* device = alcOpenDevice(NULL); // Open default device
    if (!device) {
        // Handle error
    }

    ALCcontext* context = alcCreateContext(device, NULL);
    if (!context) {
        // Handle error
    }

    alcMakeContextCurrent(context);
    
    return true;
}

void AudioOpenAL::Shutdown() {
    
}

void AudioOpenAL::Load(std::string filename, PathType pathType) {
    
}

void AudioOpenAL::PlayOnce(std::string filename) {
    
}
}   // namespace snowpulse
