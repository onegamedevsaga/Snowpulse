#include "atlas_manager.h"

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

std::shared_ptr<AtlasManager> AtlasManager::Create() {
    auto atlasMgr = new AtlasManager();
    return std::shared_ptr<AtlasManager>(atlasMgr);
}

AtlasManager::AtlasManager() : isWorking_(false) {
}

AtlasManager::~AtlasManager() {
    if (workerThread_.joinable()) {
        workerThread_.join();
    }
}

void AtlasManager::Load(std::string atlasFilename, PathType pathType) {
    if (atlases_.count(atlasFilename)) {
        // Atlas already loaded.
        return;
    }

    auto atlasFile = JsonUtils::LoadFile(atlasFilename, pathType);
    if (atlasFile) {
        std::vector<AtlasSprite> sprites;
        for (auto& pair : atlasFile->items()) {
            auto textureFilename = Directory::GetInstance()->GetPathFromFilename(atlasFilename) + pair.key();
            Application::GetInstance()->GetGraphics()->LoadTexture(textureFilename, pathType);
            auto items = pair.value();
            for (const auto& item : items) {
                AtlasSprite sprite(atlasFilename,
                                   textureFilename,
                                   item["filename"],
                                   Vector2((float)item["u"], (float)item["v"]),
                                   Vector2((float)item["width"], (float)item["height"]));
                sprites.push_back(sprite);
            }
        }
        atlases_[atlasFilename] = sprites;
    }
}

void AtlasManager::Create(Vector2Int size, std::string outputFilename, std::vector<std::string> textureFilenames, PathType texturesPathType, PathType outputPathType, std::function<void(int)> onProgressFunc) {
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
    workerThread_ = std::thread(&AtlasManager::LoadAndPackTextures, this, size, outputFilename, textureFilenames, texturesPathType, outputPathType, jsonFile_.get());
    isWorking_ = true;
}

void AtlasManager::CreateInMemory(Vector2Int size, std::string atlasFilename, std::map<std::string, unsigned char*> bitmaps, std::map<std::string, Vector2Int> sizes) {
    std::vector<std::string> filenames;
    std::vector<unsigned char*> images;
    std::vector<stbrp_rect> allRects;
    int i = 0;
    for (const auto& pair : bitmaps) {
        auto filename = pair.first;
        auto width = sizes[filename].x;
        auto height = sizes[filename].y;
        filenames.push_back(filename);
        images.push_back(pair.second);
        allRects.push_back({static_cast<stbrp_coord>(i), static_cast<stbrp_coord>(width), static_cast<stbrp_coord>(height), 0, 0, 0});
        if (width + kSpacing * 2 >= size.x || height + kSpacing * 2 >= size.y) {
#ifdef SPDEBUG
            std::cerr << "Error: Atlas Manager: Bitmap \"" << filename << "\" is bigger than the atlas size (" << size.x << ", " << size.y << ")."  << std::endl;
#endif
            return;
        }
        i++;
    }

    std::vector<stbrp_rect> rectsToPack(allRects);
    allRects.clear();
    for (const auto& rect : rectsToPack) {
        auto spacedRect = AddSpacingToRect(RectSTB(rect.id, (float)rect.x, (float)rect.y, (float)rect.w, (float)rect.h, rect.was_packed), kSpacing);
        allRects.push_back({static_cast<stbrp_coord>(spacedRect.GetId()), static_cast<stbrp_coord>(spacedRect.GetWidth()), static_cast<stbrp_coord>(spacedRect.GetHeight()), static_cast<stbrp_coord>(spacedRect.GetX()), static_cast<stbrp_coord>(spacedRect.GetY()), 0});
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
            std::cout << "Atlas Manager: Creating additional atlas..." << std::endl;
#endif
        }

        // Separate packed and unpacked rectangles
        std::vector<RectSTB> packedRects;
        std::vector<stbrp_rect> unpackedRects;
        for (const auto& rect : allRects) {
            if (rect.was_packed) {
                packedRects.push_back(RectSTB(rect.id, (float)rect.x, (float)rect.y, (float)rect.w, (float)rect.h, rect.was_packed));
            } else {
                unpackedRects.push_back(rect);
            }
        }
        // Save the packed atlas
        PackAndSaveAtlasInMemory(packedRects, filenames, images, size, atlasIndex, atlasFilename);
        allRects = unpackedRects;
        atlasIndex ++;
    }
}

AtlasSprite AtlasManager::GetSprite(std::string spriteFilename) {
    for (const auto& pair : atlases_) {
        auto spr = GetSprite(pair.first, spriteFilename);
        if (spr.IsValid()) {
            return spr;
        }
    }
    return AtlasSprite();
}

AtlasSprite AtlasManager::GetSprite(std::string atlasFilename, std::string spriteFilename) {
    if (!atlases_.count(atlasFilename)) {
#ifdef SPDEBUG
        std::cerr << "Error: Atlas doesn't have \"" << atlasFilename << "\" atlas."  << std::endl;
#endif
        return AtlasSprite();
    }

    auto sprites = atlases_[atlasFilename];
    for (const auto& sprite : sprites) {
        if (sprite.GetFilename() == spriteFilename) {
            return sprite;
        }
    }
    return AtlasSprite();
}

void AtlasManager::CheckWorkerThread() {
    {
        std::lock_guard<std::mutex> lock(progressMutex_);
        if (progress_ == 100 && workerThread_.joinable()) {
            workerThread_.join();

            JsonUtils::SaveFile(jsonFile_.get(), atlasOutputFilename_, atlasOutputPathType_);
#ifdef SPDEBUG
            std::cout << "Atlas Manager: Output:" << std::endl << jsonFile_->dump(4) << std::endl;
#endif
            isWorking_ = false;
        }
        if (onProgressFunc_ && lastReportedProgress_ != progress_) {
            onProgressFunc_(progress_);
            lastReportedProgress_ = progress_;
        }
    }
}

void AtlasManager::LoadAndPackTextures(Vector2Int size, std::string outputFilename, std::vector<std::string> textureFilenames, PathType texturesPathType, PathType outputPathType, Json* jsonFile) {
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
        unsigned char* data = stbi_load(Directory::GetInstance()->GetFullFilename(textureFilenames[i], texturesPathType).c_str(), &width, &height, &nrChannels, 4);
        if (data) {
            filenames.push_back(Directory::GetInstance()->GetFilenameFromPath(textureFilenames[i]));
            images.push_back(data);
            allRects.push_back({static_cast<stbrp_coord>(i), static_cast<stbrp_coord>(width), static_cast<stbrp_coord>(height), 0, 0, 0});
            if (width + kSpacing * 2 >= size.x || height + kSpacing * 2 >= size.y) {
#ifdef SPDEBUG
                std::cerr << "Error: Atlas Manager: Image \"" << textureFilenames[i] << "\" is bigger than the atlas size (" << size.x << ", " << size.y << ")."  << std::endl;
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
            std::cerr << "Error: Atlas Manager: Failed to load image: " << textureFilenames[i] << std::endl;
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
        auto spacedRect = AddSpacingToRect(RectSTB(rect.id, rect.x, rect.y, rect.w, rect.h, rect.was_packed), kSpacing);
        allRects.push_back({static_cast<stbrp_coord>(spacedRect.GetId()), static_cast<stbrp_coord>(spacedRect.GetWidth()), static_cast<stbrp_coord>(spacedRect.GetHeight()), static_cast<stbrp_coord>(spacedRect.GetX()), static_cast<stbrp_coord>(spacedRect.GetY()), 0});
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
            std::cout << "Atlas Manager: Creating additional atlas..." << std::endl;
#endif
        }

        // Separate packed and unpacked rectangles
        std::vector<RectSTB> packedRects;
        std::vector<stbrp_rect> unpackedRects;
        for (const auto& rect : allRects) {
            if (rect.was_packed) {
                packedRects.push_back(RectSTB(rect.id, rect.x, rect.y, rect.w, rect.h, rect.was_packed));
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

void AtlasManager::CleanImages(std::vector<unsigned char*>& images) {
    for (auto* img : images) {
        stbi_image_free(img);
    }
    images.clear();
}

void AtlasManager::PackAndSaveAtlas(const std::vector<RectSTB>& rects, const std::vector<std::string>& filenames, const std::vector<unsigned char*>& images, Vector2Int atlasSize, int atlasIndex, std::string outputFilename, PathType pathType, Json* jsonFile) {
    // Create an image buffer (RGBA) for the atlas
    std::vector<unsigned char> buffer(atlasSize.x * atlasSize.y * 4, 0);

    // Draw the images into the buffer
    Json jsonRects;
    for (auto& rect : rects) {
        if (rect.IsPacked()) {
            Json json = {   { "filename", filenames[rect.GetId()] },
                            { "u", (float)(rect.GetX() + kSpacing) / (float)atlasSize.x },
                            { "v", (float)(rect.GetY() + kSpacing) / (float)atlasSize.y },
                            { "width", (float)(rect.GetWidth() - kSpacing * 2) },
                            { "height", (float)(rect.GetHeight() - kSpacing * 2) }
            };
            jsonRects.push_back(json);

            unsigned char* img = images[rect.GetId()];
            for (int y = 0; y < rect.GetHeight() - kSpacing * 2; ++y) {
                for (int x = 0; x < rect.GetWidth() - kSpacing * 2; ++x) {
                    int buffer_index = ((rect.GetY() + y + kSpacing) * atlasSize.x + (rect.GetX() + x + kSpacing)) * 4;
                    int image_index = (y * (rect.GetWidth() - kSpacing * 2) + x) * 4;
                    for (int c = 0; c < 4; ++c) {
                        buffer[buffer_index + c] = img[image_index + c];
                    }
                }
            }
        }
    }
    // Save to PNG
    std::string filename = Directory::GetInstance()->GetFilenameFromPath(outputFilename) + "_" + std::to_string(atlasIndex) + ".png";
    std::string fullFilename = Directory::GetInstance()->GetFullFilename(outputFilename + "_" + std::to_string(atlasIndex) + ".png", pathType);
    if (!stbi_write_png(fullFilename.c_str(), atlasSize.x, atlasSize.y, 4, buffer.data(), atlasSize.x * 4)) {
#ifdef SPDEBUG
        std::cerr << "Error: Atlas Manager: Failed to write image: " << fullFilename << std::endl;
#endif
    }
    else {
#ifdef SPDEBUG
        std::cout << "Atlas Manager: Image saved as " << fullFilename << std::endl;
#endif
    }
    (*jsonFile)[filename] = jsonRects;
}

void AtlasManager::PackAndSaveAtlasInMemory(const std::vector<RectSTB>& rects, const std::vector<std::string>& filenames, const std::vector<unsigned char*>& bitmaps, Vector2Int atlasSize, int atlasIndex, std::string atlasFilename) {
    std::vector<unsigned char> buffer(atlasSize.x * atlasSize.y * 4, 0);
    auto textureFilename = atlasFilename + "_" + std::to_string(atlasIndex);
    for (auto& rect : rects) {
        if (rect.IsPacked()) {
            unsigned char* bitmap = bitmaps[rect.GetId()];
            for (int y = 0; y < (int)rect.GetHeight() - kSpacing * 2; ++y) {
                for (int x = 0; x < (int)rect.GetWidth() - kSpacing * 2; ++x) {
                    int buffer_index = (((int)rect.GetY() + y + kSpacing) * atlasSize.x + ((int)rect.GetX() + x + kSpacing)) * 4;
                    int image_index = (y * ((int)rect.GetWidth() - kSpacing * 2) + x) * 4;
                    for (int c = 0; c < 4; ++c) {
                        buffer[buffer_index + c] = bitmap[image_index + c];
                    }
                }
            }

            auto atlasSprite = AtlasSprite( atlasFilename,
                                            textureFilename,
                                            filenames[rect.GetId()],
                                            Vector2((rect.GetX() + (float)kSpacing) / (float)atlasSize.x,
                                                    (rect.GetY() + (float)kSpacing) / (float)atlasSize.y),
                                            Vector2((rect.GetWidth() - (float)kSpacing * 2),
                                                    (rect.GetHeight() - (float)kSpacing * 2)));
            atlases_[atlasFilename].push_back(atlasSprite);
        }
    }

    auto graphics = Application::GetInstance()->GetGraphics();
    graphics->LoadTexture(textureFilename, buffer.data(), atlasSize);

    // Save to PNG
    std::string fullFilename = Directory::GetInstance()->GetFullFilename(atlasFilename + "_" + std::to_string(atlasIndex) + ".png", PathType::kDocuments);
    if (!stbi_write_png(fullFilename.c_str(), atlasSize.x, atlasSize.y, 4, buffer.data(), atlasSize.x * 4)) {
#ifdef SPDEBUG
        std::cerr << "Error: Atlas Manager: Failed to write image: " << fullFilename << std::endl;
#endif
    }
    else {
#ifdef SPDEBUG
        std::cout << "Atlas Manager: Image saved as " << fullFilename << std::endl;
#endif
    }
}

RectSTB AtlasManager::AddSpacingToRect(const RectSTB& rect, int spacing) {
    return RectSTB(rect.GetId(), rect.GetPosition(), Vector2(rect.GetWidth() + spacing * 2.0f, rect.GetHeight() + spacing * 2.0f), rect.IsPacked());
}
}   // namespace snowpulse
