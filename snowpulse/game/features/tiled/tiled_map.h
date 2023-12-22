#ifndef SNOWPULSE_GAME_FEATURES_TILED_TILEDMAP_H_
#define SNOWPULSE_GAME_FEATURES_TILED_TILEDMAP_H_

#include "../../../defines.h"
#include "../../updatable.h"

#include <memory>
#include <string>
#include <map>
#include <vector>

#include "tiled_data_map.h"
#include "tiled_data_tile.h"
#include "../../../common/json_utils.h"
#include "../../../common/texture_filtering.h"
#include "../../../common/path_type.h"
#include "../../../common/matrix4x4.h"
#include "../../../application/graphics.h"

namespace snowpulse {
class SNOWPULSEAPI TiledMap : public Updatable {
    public:
        static std::shared_ptr<TiledMap> Create(std::string jsonFilename, PathType pathType, TextureFiltering filtering);

        virtual ~TiledMap();
        // From Updatable
        void Update(float deltaTime);

        void AddTileset(std::string jsonFilename);
        void Draw(Graphics* graphics, Matrix4x4 worldMatrix, int sortOrder, bool isPremultiplied);

        TiledDataTile GetTile(int tileId);
        TiledDataTile GetTile(int x, int y, int layerId);

    protected:
        TiledMap();
        void LoadJson();

        std::string jsonFilename_;
        PathType pathType_;
        TextureFiltering textureFiltering_;
        TiledDataMap mapData_;
};
}   // namespace snowpulse
#endif
