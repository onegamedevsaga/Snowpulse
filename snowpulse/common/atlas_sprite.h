#ifndef SNOWPULSE_COMMON_ATLASSPRITE_H_
#define SNOWPULSE_COMMON_ATLASSPRITE_H_

#include "../defines.h"

#include <string>

#include "vector2.h"

namespace snowpulse {
class SNOWPULSEAPI AtlasSprite {
    public:
        AtlasSprite() : isValid_(false), atlasFilename_(""), textureFilename_(""), filename_(""), uv_(), size_() { }
        AtlasSprite(std::string atlasFilename, std::string textureFilename, std::string filename, Vector2 uv, Vector2 size) : isValid_(true), atlasFilename_(atlasFilename), textureFilename_(textureFilename), filename_(filename), uv_(uv), size_(size) { }

        bool IsValid() const { return isValid_; }
        std::string GetAtlasFilename() const { return atlasFilename_; }
        std::string GetTextureFilename() const { return textureFilename_; }
        std::string GetFilename() const { return filename_; }
        Vector2 GetUV() const { return uv_; }
        Vector2 GetSize() const { return size_; }

    private:
        bool isValid_;
        std::string atlasFilename_;
        std::string textureFilename_;
        std::string filename_;
        Vector2 uv_;
        Vector2 size_;
};
}   // namespace snowpulse
#endif
