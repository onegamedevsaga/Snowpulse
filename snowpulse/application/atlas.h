#ifndef SNOWPULSE_APPLICATION_ATLAS_H_
#define SNOWPULSE_APPLICATION_ATLAS_H_

#include "../defines.h"

#include <memory>

namespace snowpulse {

class SNOWPULSEAPI Atlas {
    public:
        static std::shared_ptr<Atlas> Create();

        virtual ~Atlas();

    private:
        Atlas();
};
}   // namespace snowpulse
#endif
