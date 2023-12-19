#ifndef SNOWPULSE_GAME_PLUGINS_TILED_TILEDDATAMAP_H_
#define SNOWPULSE_GAME_PLUGINS_TILED_TILEDDATAMAP_H_

#include "../../../defines.h"
#include "../../updatable.h"

#include <memory>
#include <string>
#include <vector>

#include "../../../common/color.h"
#include "../../../common/vector2int.h"
#include "tiled_data_layer.h"

namespace snowpulse {
class SNOWPULSEAPI TiledDataMap {
    public:
        std::string version;
        std::string type;
        Color backgroundColor;
        std::string className;
        int compressionLevel;
        Vector2Int size;
        bool isInfinite;
        std::string orientation;
        std::string renderOrder;
        std::string tiledVersion;
        Vector2Int tileSize;
        std::vector<TiledDataLayer> layers;
};
}   // namespace snowpulse
#endif
