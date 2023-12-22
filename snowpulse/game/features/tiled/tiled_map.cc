#include "tiled_map.h"

#include <iostream>
#include <map>

#include "../../../common/color.h"
#include "../../../application/atlas_manager.h"

namespace snowpulse {
std::shared_ptr<TiledMap> TiledMap::Create(std::string jsonFilename, PathType pathType, TextureFiltering filtering) {
    auto tiledMap = std::shared_ptr<TiledMap>(new TiledMap());
    tiledMap->jsonFilename_ = jsonFilename;
    tiledMap->pathType_ = pathType;
    tiledMap->textureFiltering_ = filtering;
    tiledMap->LoadJson();
    return tiledMap;
}

TiledMap::TiledMap() {
    
}

TiledMap::~TiledMap() {
    
}

void TiledMap::Update(float deltaTime) {
    
}

void TiledMap::AddTileset(std::string jsonFilename) {
    
}

void TiledMap::Draw(Graphics* graphics, Matrix4x4 worldMatrix, int sortOrder, bool isPremultiplied) {
}

TiledDataTile TiledMap::GetTile(int tileId) {
    TiledDataTile tile;
    return tile;
}

TiledDataTile TiledMap::GetTile(int x, int y, int layerId) {
    TiledDataTile tile;
    auto layers = mapData_.layers;
    for (const auto& l : layers) {
        if (l.layerId == layerId) {
            auto tileId = l.data[y * l.size.x + x];
            return GetTile(tileId);
        }
    }
    return tile;
}

void TiledMap::LoadJson() {
    auto json = JsonUtils::LoadFile(jsonFilename_, pathType_);
    if (json->contains("version")) {
        mapData_.version = (*json)["version"];
    }
    if (json->contains("type")) {
        mapData_.type = (*json)["type"];
    }
    if (json->contains("backgroundcolor")) {
        std::string color = (*json)["backgroundcolor"];
        if (color.size() == 7) {
            color += "ff";
        }
        else if (color.size() == 9) {
            color = "#" + color.substr(3, 6) + color.substr(1, 2);
        }
        mapData_.backgroundColor = Color(color);
    }
    if (json->contains("class")) {
        mapData_.className = (*json)["class"];
    }
    if (json->contains("compressionlevel")) {
        mapData_.compressionLevel = (*json)["compressionlevel"];
    }
    if (json->contains("width") && json->contains("height")) {
        mapData_.size.x = (*json)["width"];
        mapData_.size.y = (*json)["height"];
    }
    if (json->contains("infinite")) {
        mapData_.isInfinite = (*json)["infinite"];
    }
    if (json->contains("orientation")) {
        mapData_.orientation = (*json)["orientation"];
    }
    if (json->contains("renderorder")) {
        mapData_.renderOrder = (*json)["renderorder"];
    }
    if (json->contains("tiledversion")) {
        mapData_.tiledVersion = (*json)["tiledversion"];
    }
    if (json->contains("tilewidth") && json->contains("tileheight")) {
        mapData_.tileSize.x = (*json)["tilewidth"];
        mapData_.tileSize.y = (*json)["tileheight"];
    }
    if (json->contains("layers")) {
        auto layers = (*json)["layers"];
        mapData_.layers.reserve(layers.size());
        for (const auto& l : layers) {
            TiledDataLayer layer;
            if (l.contains("id")) {
                layer.layerId = l.contains("id");
            }
            if (l.contains("width") && json->contains("height")) {
                layer.size.x = l["width"];
                layer.size.y = l["height"];
            }
            if (l.contains("x") && json->contains("y")) {
                layer.offsetInTiles.x = l["x"];
                layer.offsetInTiles.y = l["y"];
            }
            if (l.contains("name")) {
                layer.name = l["name"];
            }
            if (l.contains("type")) {
                layer.type = l["type"];
            }
            if (l.contains("visible")) {
                layer.isVisible = l["visible"];
            }
            if (l.contains("compression")) {
                layer.compression = l["compression"];
            }
            if (l.contains("encoding")) {
                layer.encoding = l["encoding"];
            }
            if (l.contains("opacity")) {
                layer.opacity = l["opacity"];
            }
            if (l.contains("data")) {
                auto rawData = l["data"];
                if (layer.compression == "csv") {
                    for (const auto& ld : rawData) {
                        layer.data.push_back(ld);
                    }
                }
            }
            mapData_.layers.push_back(layer);
        }
    }

    auto atlasManager = Application::GetInstance()->GetAtlasManager();
    atlasManager->LoadTiles("sample.png", Vector2Int(3500 / 2, 2000), 20, 0);
}
}   // namespace snowpulse
