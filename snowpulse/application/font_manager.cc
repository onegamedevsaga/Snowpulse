#include "font_manager.h"

#include <iostream>
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>
#undef STB_TRUETYPE_IMPLEMENTATION

#include "application.h"
#include "directory.h"
#include "atlas_manager.h"
#include "../common/json_utils.h"
#include "../common/vector2int.h"

namespace snowpulse {
static const int kSpacing = 4;

std::shared_ptr<FontManager> FontManager::Create() {
    auto fontMgr = new FontManager();
    return std::shared_ptr<FontManager>(fontMgr);
}

FontManager::FontManager() : atlasManager_(SPNULL) {
}

FontManager::~FontManager() {
}

void FontManager::Load(std::string filename, int fontSizeInPixels, PathType pathType) {
    if (!atlasManager_) {
        atlasManager_ = Application::GetInstance()->GetAtlasManager();
    }

    auto fullFilename = Directory::GetInstance()->GetFullFilename(filename, pathType);
    unsigned char* buffer = SPNULL;
    size_t bufferSize;
    FILE* fontFile = fopen(fullFilename.c_str(), "rb");
    if (fontFile) {
        fseek(fontFile, 0, SEEK_END);
        bufferSize = ftell(fontFile);
        fseek(fontFile, 0, SEEK_SET);
        buffer = new unsigned char[bufferSize];
        fread(buffer, 1, bufferSize, fontFile);
        fclose(fontFile);
    }
    else {
#ifdef SPDEBUG
        std::cerr << "Error: Font Manager: " << filename << " not found." << std::endl;
#endif
        return;
    }

    stbtt_fontinfo fontInfo;
    if (!stbtt_InitFont(&fontInfo, buffer, 0)) {
#ifdef SPDEBUG
        std::cerr << "Error: Font Manager: " << filename << " can't be initialized." << std::endl;
#endif
    }

    std::map<char, unsigned char*> bitmaps;
    float scale = stbtt_ScaleForPixelHeight(&fontInfo, fontSizeInPixels);

    std::string characters = "abcdefghijklmnopqrstuvwxyz0123456789.,!?@";
    for (const auto& c : characters) {
        int width, height, xOffset, yOffset;
        unsigned char* bitmap = stbtt_GetCodepointBitmap(&fontInfo, 0, scale, c, &width, &height, &xOffset, &yOffset);
        spriteNames_[filename][c] = filename + "_" + c + "_" + std::to_string(fontSizeInPixels);
        sizes_[filename][c] = Vector2Int(width, height);
        offsets_[filename][c] = Vector2Int(xOffset, yOffset);
        bitmaps[c] = bitmap;
    }

    std::map<std::string, unsigned char*> nameAndBitmaps;
    std::map<std::string, Vector2Int> nameAndSizes;
    for (const auto& pair : bitmaps) {
        auto spriteName = spriteNames_[filename][pair.first];
        nameAndBitmaps[spriteName] = pair.second;
        nameAndSizes[spriteName] = sizes_[filename][pair.first];
    }

    atlasManager_->CreateInMemory(Vector2Int(2048, 2048), filename, nameAndBitmaps, nameAndSizes);

    for (const auto& pair : bitmaps) {
        delete [] pair.second;
    }
    delete [] buffer;
}
}   // namespace snowpulse
