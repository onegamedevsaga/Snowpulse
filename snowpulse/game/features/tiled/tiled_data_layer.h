#ifndef SNOWPULSE_GAME_FEATURES_TILED_TILEDDATALAYER_H_
#define SNOWPULSE_GAME_FEATURES_TILED_TILEDDATALAYER_H_

#include "../../../defines.h"
#include "../../updatable.h"

#include <memory>
#include <string>
#include <vector>

#include "../../../common/vector2int.h"

namespace snowpulse {
class SNOWPULSEAPI TiledDataLayer {
    public:
        int layerId;
        std::string name;
        std::string type;
        bool isVisible;
        std::string compression = "csv";
        std::string encoding;
        Vector2Int size;
        Vector2Int offsetInTiles;
        Vector2Int offsetInPixels;
        float opacity;
        std::vector<int> data;
};
}   // namespace snowpulse
#endif
