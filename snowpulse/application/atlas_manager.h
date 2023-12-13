#ifndef SNOWPULSE_APPLICATION_ATLASMANAGER_H_
#define SNOWPULSE_APPLICATION_ATLASMANAGER_H_

#include "../defines.h"

#include <vector>
#include <map>
#include <string>
#include <stb_rect_pack.h>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>

#include "../common/path_type.h"
#include "../common/vector2int.h"
#include "../common/json_utils.h"
#include "../common/atlas_sprite.h"

namespace snowpulse {

class SNOWPULSEAPI AtlasManager {
    public:
        static std::shared_ptr<AtlasManager> Create();
        virtual ~AtlasManager();

        void Load(std::string atlasFilename, PathType pathType = PathType::kAssets);
        void Create(Vector2Int size, std::string outputFilename, std::vector<std::string> textureFilenames, PathType texturesPathType, PathType outputPathType, std::function<void(int)> onProgressFunc);
        AtlasSprite GetSprite(std::string spriteFilename);
        AtlasSprite GetSprite(std::string atlasFilename, std::string spriteFilename);
        void CheckWorkerThread();

        bool IsWorking() const { return isWorking_;}

    protected:
        AtlasManager();

        void LoadAndPackTextures(Vector2Int size, std::string outputFilename, std::vector<std::string> textureFilenames, PathType texturesPathType, PathType outputPathType, Json* jsonFile);
        void CleanImages(std::vector<unsigned char*>& images);
        void PackAndSaveAtlas(const std::vector<stbrp_rect>& rects, const std::vector<std::string>& filenames, const std::vector<unsigned char*>& images, Vector2Int atlasSize, int atlasIndex, std::string outputFilename, PathType pathType, Json* jsonFile);
        std::string GetFullFilename(std::string filename, PathType pathType);
        stbrp_rect AddSpacingToRect(const stbrp_rect& rect, int spacing);

        std::map<std::string, std::vector<AtlasSprite>> atlases_;
        bool isWorking_;
        int lastReportedProgress_;
        std::thread workerThread_;
        std::mutex progressMutex_;
        std::atomic<int> progress_;
        std::function<void(int)> onProgressFunc_;
        std::shared_ptr<Json> jsonFile_;
        std::string atlasOutputFilename_;
        PathType atlasOutputPathType_;
};
}   // namespace snowpulse
#endif
