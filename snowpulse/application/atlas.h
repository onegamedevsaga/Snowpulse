#ifndef SNOWPULSE_APPLICATION_ATLAS_H_
#define SNOWPULSE_APPLICATION_ATLAS_H_

#include "../defines.h"

#include <vector>
#include <string>
#include <stb_rect_pack.h>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>

#include "../common/path_type.h"
#include "../common/vector2int.h"

namespace snowpulse {

class SNOWPULSEAPI Atlas {
    public:
        static std::shared_ptr<Atlas> Create();
        virtual ~Atlas();

        virtual void Load(std::string atlasFilename, PathType pathType = PathType::kAssets);
        virtual void Create(Vector2Int size, std::string outputFilename, std::vector<std::string> textureFilenames, PathType texturesPathType, PathType outputPathType, std::function<void(int)> onProgressFunc);

        void CheckWorkerThread();

        bool IsWorking() const { return isWorking_;}

    protected:
        Atlas();

        void LoadAndPackTextures(Vector2Int size, std::string outputFilename, std::vector<std::string> textureFilenames, PathType texturesPathType, PathType outputPathType);
        void CleanImages(std::vector<unsigned char*>& images);
        void PackAndSaveAtlas(const std::vector<stbrp_rect>& rects, const std::vector<unsigned char*>& images, Vector2Int atlasSize, int atlasIndex, std::string outputFilename, PathType pathType);
        std::string GetFullFilename(std::string filename, PathType pathType);
        stbrp_rect AddSpacingToRect(const stbrp_rect& rect, int spacing);

        bool isWorking_;
        int lastReportedProgress_;
        std::thread workerThread_;
        std::mutex progressMutex_;
        std::atomic<int> progress_;
        std::function<void(int)> onProgressFunc_;
};
}   // namespace snowpulse
#endif
