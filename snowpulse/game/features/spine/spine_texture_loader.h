#ifndef SNOWPULSE_GAME_FEATURES_SPINE_SPINETEXTURELOADER_H_
#define SNOWPULSE_GAME_FEATURES_SPINE_SPINETEXTURELOADER_H_

#include "../../../defines.h"
#include <spine/TextureLoader.h>

#include <memory>
#include <string>

#include "../../../application/graphics.h"
#include "../../../common/texture_filtering.h"

namespace snowpulse {
class SNOWPULSEAPI SpineTextureLoader : public spine::TextureLoader {
    public:
        static std::shared_ptr<SpineTextureLoader> Create();

        virtual ~SpineTextureLoader();

        // From TextureLoader
        void load(spine::AtlasPage &page, const spine::String &path);
        void unload(void *texture);

    private:
        SpineTextureLoader();

        Graphics* graphics_;
};
}   // namespace snowpulse
#endif
