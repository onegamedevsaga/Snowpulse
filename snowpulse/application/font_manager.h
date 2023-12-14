#ifndef SNOWPULSE_APPLICATION_FONTMANAGER_H_
#define SNOWPULSE_APPLICATION_FONTMANAGER_H_

#include "../defines.h"

#include <memory>
#include <string>
#include <map>

#include "../common/path_type.h"
#include "../common/vector2int.h"

namespace snowpulse {

// Forward declaration
class AtlasManager;

class SNOWPULSEAPI FontManager {
    public:
        static std::shared_ptr<FontManager> Create();
        virtual ~FontManager();

        void Load(std::string filename, int fontSizeInPixels, PathType pathType);

    private:
        FontManager();

        AtlasManager* atlasManager_;
        std::map<std::string, std::map<char, std::string>> spriteNames_;
        std::map<std::string, std::map<char, Vector2Int>> sizes_;
        std::map<std::string, std::map<char, Vector2Int>> offsets_;
};
}   // namespace snowpulse
#endif
