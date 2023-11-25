#ifndef SNOWPULSE_APPLICATION_DIRECTORY_H_
#define SNOWPULSE_APPLICATION_DIRECTORY_H_

#include "../defines.h"

#include <string>

namespace snowpulse {
class SNOWPULSEAPI Directory {
    public:
        static Directory* GetInstance();

        virtual ~Directory();
        virtual std::string GetAssetsPath(std::string filename) = 0;

    protected:
        Directory();
};
}   // namespace snowpulse

#endif
