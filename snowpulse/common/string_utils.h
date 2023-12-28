#ifndef SNOWPULSE_COMMON_STRINGUTILS_H_
#define SNOWPULSE_COMMON_STRINGUTILS_H_

#include "../defines.h"

#include <string>

namespace snowpulse {
class SNOWPULSEAPI StringUtils {
    public:
        static std::string ToLower(std::string str);
        static std::string ToUpper(std::string str);
};
}   // namespace snowpulse
#endif
