#ifndef SNOWPULSE_APPLICATION_OPENAL_AUDIOOPENAL_H_
#define SNOWPULSE_APPLICATION_OPENAL_AUDIOOPENAL_H_

#include "../../defines.h"
#include "../audio.h"

#include <memory>
#include <map>

namespace snowpulse {

class SNOWPULSEAPI AudioOpenAL : public Audio {
    public:
        static std::shared_ptr<AudioOpenAL> Create();

        ~AudioOpenAL();
        bool Initialize();

        //RenderQueueOpenGL* GetRenderQueue() const { return renderQueue_.get(); }

        // Audio
        void Shutdown() override;
        void Load(std::string filename, PathType pathType) override;
        void PlayOnce(std::string filename) override;

    private:
        AudioOpenAL();

};
}   // namespace snowpulse
#endif
