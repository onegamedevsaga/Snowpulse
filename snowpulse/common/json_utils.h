#ifndef SNOWPULSE_COMMON_JSONUTILS_H_
#define SNOWPULSE_COMMON_JSONUTILS_H_

#include "../defines.h"

#include <memory>
#include <string>

#include "../../thirdparty/nlohmann-json/json.hpp"
#include "../application/application.h"
#include "path_type.h"

namespace snowpulse {

typedef nlohmann::json Json;

class SNOWPULSEAPI JsonUtils {
    public:
        static std::shared_ptr<Json> Create();
        static std::shared_ptr<Json> LoadFile(std::string filename, PathType pathType);
        static bool SaveFile(Json* data, std::string filename, PathType pathType = PathType::kApplicationSupport);
};
}   // namespace snowpulse
#endif
