#ifndef SNOWPULSE_GAME_UPDATABLE_H_
#define SNOWPULSE_GAME_UPDATABLE_H_

#include "../defines.h"

namespace snowpulse {
class SNOWPULSEAPI Updatable {
    public:
        virtual ~Updatable() = default;
        virtual void Update(float deltaTime) = 0;
};
}   // namespace snowpulse

#endif
