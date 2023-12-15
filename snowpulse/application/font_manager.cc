#include "font_manager.h"

#include <iostream>
#include <stb_image_write.h>
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

    auto fontFilename = filename + "_" + std::to_string(fontSizeInPixels);
    auto fullFilename = Directory::GetInstance()->GetFullFilename(filename, pathType);

    if (loadedFonts_.count(fontFilename)) {
        return;
    }

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

    std::string characters =  "abcdefghijklmnopqrstuvwxyz0123456789.,!?@:- ";
                characters += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (const auto& c : characters) {
        int width, height, xOffset, yOffset;
        unsigned char* grayscaleBitmap = stbtt_GetCodepointBitmap(&fontInfo, 0, scale, c, &width, &height, &xOffset, &yOffset);
        spriteNames_[fontFilename][c] = GetCharacterSpriteName(filename, fontSizeInPixels, c);
        sizes_[fontFilename][c] = Vector2Int(width, height);
        offsets_[fontFilename][c] = Vector2Int(xOffset, yOffset);
        unsigned char* rgbaBitmap = new unsigned char[width * height * 4];
        for (int i = 0; i < width * height; ++i) {
            rgbaBitmap[i * 4 + 0] = grayscaleBitmap[i];
            rgbaBitmap[i * 4 + 1] = grayscaleBitmap[i];
            rgbaBitmap[i * 4 + 2] = grayscaleBitmap[i];
            rgbaBitmap[i * 4 + 3] = grayscaleBitmap[i];
        }
        delete [] grayscaleBitmap;
        bitmaps[c] = rgbaBitmap;
    }

    std::map<std::string, unsigned char*> nameAndBitmaps;
    std::map<std::string, Vector2Int> nameAndSizes;
    for (const auto& pair : bitmaps) {
        auto spriteName = spriteNames_[fontFilename][pair.first];
        nameAndBitmaps[spriteName] = pair.second;
        nameAndSizes[spriteName] = sizes_[fontFilename][pair.first];
    }

    atlasManager_->CreateInMemory(Vector2Int(2048, 2048), fontFilename, nameAndBitmaps, nameAndSizes);

    for (const auto& pair : bitmaps) {
        delete [] pair.second;
    }
    delete [] buffer;
    loadedFonts_[fontFilename] = true;
}

std::string FontManager::GetCharacterSpriteName(std::string filename, int fontSizeInPixels, char character) {
    return Directory::GetInstance()->GetFilenameFromPath(filename) + "_" + character + "_" + std::to_string(fontSizeInPixels);
}

Vector2 FontManager::GetCharacterOffset(std::string filename, int fontSizeInPixels, char character) {
    auto fontFilename = filename + "_" + std::to_string(fontSizeInPixels);
    if (!offsets_.count(fontFilename) || !offsets_[fontFilename].count(character)) {
        return Vector2(-1.0f, -1.0f);
    }

    auto offset = offsets_[fontFilename][character];
    return Vector2((float)offset.x, (float)offset.y);
}
}   // namespace snowpulse
