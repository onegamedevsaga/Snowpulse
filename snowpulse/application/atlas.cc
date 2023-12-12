#include "atlas.h"

#include <iostream>
#include <stb_image.h>
#define STB_RECT_PACK_IMPLEMENTATION
#include <stb_rect_pack.h>
#undef STB_RECT_PACK_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#undef STB_IMAGE_WRITE_IMPLEMENTATION

#include "directory.h"
#include "../common/json_utils.h"

namespace snowpulse {
static const int kSpacing = 4;

std::shared_ptr<Atlas> Atlas::Create() {
    auto atlas = new Atlas();
    return std::shared_ptr<Atlas>(atlas);
}

Atlas::Atlas() : isWorking_(false) {
}

Atlas::~Atlas() {
    if (workerThread_.joinable()) {
        workerThread_.join();
    }
}

void Atlas::Load(std::string atlasFilename, PathType pathType) {
}

void Atlas::Create(Vector2Int size, std::string outputFilename, std::vector<std::string> textureFilenames, PathType texturesPathType, PathType outputPathType, std::function<void(int)> onProgressFunc) {
    if (isWorking_) {
#ifdef SPDEBUG
        std::cerr << "Error: Atlas still working.."  << std::endl;
#endif
        return;
    }

    jsonFile_ = JsonUtils::Create();
    atlasOutputFilename_ = outputFilename + ".snowatlas";
    atlasOutputPathType_ = outputPathType;
    onProgressFunc_ = onProgressFunc;
    lastReportedProgress_ = 0;
    if (onProgressFunc_) {
        onProgressFunc_(progress_);
    }
    workerThread_ = std::thread(&Atlas::LoadAndPackTextures, this, size, outputFilename, textureFilenames, texturesPathType, outputPathType, jsonFile_.get());
    isWorking_ = true;
}

void Atlas::CheckWorkerThread() {
    {
        std::lock_guard<std::mutex> lock(progressMutex_);
        if (progress_ == 100 && workerThread_.joinable()) {
            workerThread_.join();

            JsonUtils::SaveFile(jsonFile_.get(), atlasOutputFilename_, atlasOutputPathType_);
#ifdef SPDEBUG
            std::cout << "Atlas: Output:" << std::endl << jsonFile_->dump(4) << std::endl;
#endif
            isWorking_ = false;
        }
        if (onProgressFunc_ && lastReportedProgress_ != progress_) {
            onProgressFunc_(progress_);
            lastReportedProgress_ = progress_;
        }
    }
}

void Atlas::LoadAndPackTextures(Vector2Int size, std::string outputFilename, std::vector<std::string> textureFilenames, PathType texturesPathType, PathType outputPathType, Json* jsonFile) {
    {
        std::lock_guard<std::mutex> lock(progressMutex_);
        progress_ = 0;
    }

    // Load images
    std::vector<std::string> filenames;
    std::vector<unsigned char*> images;
    std::vector<stbrp_rect> allRects;
    int progressPortion = 50 / (int)textureFilenames.size();
    for (int i = 0, n = (int)textureFilenames.size(); i < n; i++) {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(GetFullFilename(textureFilenames[i], texturesPathType).c_str(), &width, &height, &nrChannels, 4);
        if (data) {
            filenames.push_back(Directory::GetInstance()->GetFilenameFromPath(textureFilenames[i]));
            images.push_back(data);
            allRects.push_back({static_cast<stbrp_coord>(i), static_cast<stbrp_coord>(width), static_cast<stbrp_coord>(height), 0, 0, 0});
            if (width + kSpacing * 2 >= size.x || height + kSpacing * 2 >= size.y) {
#ifdef SPDEBUG
                std::cerr << "Error: Atlas: Image \"" << textureFilenames[i] << "\" is bigger than the atlas size (" << size.x << ", " << size.y << ")."  << std::endl;
#endif
                CleanImages(images);
                {
                    std::lock_guard<std::mutex> lock(progressMutex_);
                    progress_ = 100;
                }
                return;
            }
        }
        else {
#ifdef SPDEBUG
            std::cerr << "Error: Atlas: Failed to load image: " << textureFilenames[i] << std::endl;
#endif
        }

        {
            std::lock_guard<std::mutex> lock(progressMutex_);
            progress_ += progressPortion;
        }
    }

    {
        std::lock_guard<std::mutex> lock(progressMutex_);
        progress_ = 50;
    }

    std::vector<stbrp_rect> rectsToPack(allRects);
    allRects.clear();
    for (const auto& rect : rectsToPack) {
        allRects.push_back(AddSpacingToRect(rect, kSpacing));
    }

    int atlasIndex = 0;
    while (!allRects.empty()) {
        // Create a pack context
        stbrp_context context;
        std::vector<stbrp_node> nodes(size.x);
        stbrp_init_target(&context, size.x, size.y, &nodes[0], (int)nodes.size());

        // Pack rectangles
        if (!stbrp_pack_rects(&context, &allRects[0], (int)allRects.size())) {
#ifdef SPDEBUG
            std::cout << "Atlas: Creating additional atlas..." << std::endl;
#endif
        }

        // Separate packed and unpacked rectangles
        std::vector<stbrp_rect> packedRects, unpackedRects;
        for (const auto& rect : allRects) {
            if (rect.was_packed) {
                packedRects.push_back(rect);
            } else {
                unpackedRects.push_back(rect);
            }
        }

        {
            std::lock_guard<std::mutex> lock(progressMutex_);
            progress_ += 4;
            if (progress_ >= 100) {
                progress_ = 90;
            }
        }
        // Save the packed atlas
        PackAndSaveAtlas(packedRects, filenames, images, size, atlasIndex, outputFilename, outputPathType, jsonFile);
        allRects = unpackedRects;
        {
            std::lock_guard<std::mutex> lock(progressMutex_);
            progress_ += 6;
            if (progress_ >= 100) {
                progress_ = 90;
            }
        }
        atlasIndex ++;
    }

    // Clean up
    CleanImages(images);

    {
        std::lock_guard<std::mutex> lock(progressMutex_);
        progress_ = 100;
    }
}

void Atlas::CleanImages(std::vector<unsigned char*>& images) {
    for (auto* img : images) {
        stbi_image_free(img);
    }
    images.clear();
}

void Atlas::PackAndSaveAtlas(const std::vector<stbrp_rect>& rects, const std::vector<std::string>& filenames, const std::vector<unsigned char*>& images, Vector2Int atlasSize, int atlasIndex, std::string outputFilename, PathType pathType, Json* jsonFile) {
    // Create an image buffer (RGBA) for the atlas
    std::vector<unsigned char> buffer(atlasSize.x * atlasSize.y * 4, 0);

    // Draw the images into the buffer
    Json jsonRects;
    for (auto& rect : rects) {
        if (rect.was_packed) {
            Json json = {   { "filename", filenames[rect.id] },
                            { "u", (float)(rect.x + kSpacing) / (float)atlasSize.x },
                            { "v", (float)(rect.y + kSpacing) / (float)atlasSize.y },
                            { "width", (float)(rect.w - kSpacing * 2) },
                            { "height", (float)(rect.h - kSpacing * 2) }
            };
            jsonRects.push_back(json);

            unsigned char* img = images[rect.id];
            for (int y = 0; y < rect.h - kSpacing * 2; ++y) {
                for (int x = 0; x < rect.w - kSpacing * 2; ++x) {
                    int buffer_index = ((rect.y + y + kSpacing) * atlasSize.x + (rect.x + x + kSpacing)) * 4;
                    int image_index = (y * (rect.w - kSpacing * 2) + x) * 4;
                    for (int c = 0; c < 4; ++c) {
                        buffer[buffer_index + c] = img[image_index + c];
                    }
                }
            }
        }
    }
    // Save to PNG
    std::string filename = Directory::GetInstance()->GetFilenameFromPath(outputFilename) + "_" + std::to_string(atlasIndex) + ".png";
    std::string fullFilename = GetFullFilename(outputFilename + "_" + std::to_string(atlasIndex) + ".png", pathType);
    if (!stbi_write_png(fullFilename.c_str(), atlasSize.x, atlasSize.y, 4, buffer.data(), atlasSize.x * 4)) {
#ifdef SPDEBUG
        std::cerr << "Error: Atlas: Failed to write image: " << fullFilename << std::endl;
#endif
    }
    else {
#ifdef SPDEBUG
        std::cout << "Atlas: Image saved as " << fullFilename << std::endl;
#endif
    }
    (*jsonFile)[filename] = jsonRects;
}

std::string Atlas::GetFullFilename(std::string filename, PathType pathType) {
    std::string fullFilename = "";
    switch (pathType) {
        case PathType::kAssets:
            fullFilename = Directory::GetInstance()->GetAssetsPath(filename);
            break;
        case PathType::kDefaults:
            fullFilename = Directory::GetInstance()->GetDefaultsPath(filename);
            break;
        case PathType::kApplicationSupport:
            fullFilename = Directory::GetInstance()->GetApplicationSupportPath(filename);
            break;
        case PathType::kDocuments:
            fullFilename = Directory::GetInstance()->GetDocumentsPath(filename);
            break;
        case PathType::kRaw:
            fullFilename = filename;
        default:
            break;
    }
    return fullFilename;
}

stbrp_rect Atlas::AddSpacingToRect(const stbrp_rect& rect, int spacing) {
    stbrp_rect newRect = rect;
    newRect.w += spacing * 2;
    newRect.h += spacing * 2;
    return newRect;
}
}   // namespace snowpulse
