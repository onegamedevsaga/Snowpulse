#ifndef SNOWPULSE_APPLICATION_AUDIO_H_
#define SNOWPULSE_APPLICATION_AUDIO_H_

#include "../defines.h"

#include <memory>
#include <string>

#include "../common/path_type.h"

namespace snowpulse {

class SNOWPULSEAPI Audio {
    public:
        virtual ~Audio() = default;
        virtual void Shutdown() = 0;
        virtual void Load(std::string filename, PathType pathType = PathType::kAssets) = 0;
        virtual void PlayOnce(std::string filename) = 0;

    protected:
        Audio() = default;
};
}   // namespace snowpulse

#endif
