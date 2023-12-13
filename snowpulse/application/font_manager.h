#ifndef SNOWPULSE_APPLICATION_FONTMANAGER_H_
#define SNOWPULSE_APPLICATION_FONTMANAGER_H_

#include "../defines.h"

#include <memory>
#include <string>

#include "../common/path_type.h"

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
};
}   // namespace snowpulse
#endif
