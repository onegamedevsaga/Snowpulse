#include "font_manager.h"

#include <iostream>
#include <stb_image.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>
#undef STB_TRUETYPE_IMPLEMENTATION

#include "application.h"
#include "directory.h"
#include "../common/json_utils.h"

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
    unsigned char* buffer;
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

    stbtt_fontinfo fontInfo;
    if (!stbtt_InitFont(&fontInfo, buffer, 0)) {
#ifdef SPDEBUG
        std::cerr << "Error: Font Manager: " << filename << " can't be initialized." << std::endl;
#endif
    }

    float scale = stbtt_ScaleForPixelHeight(&fontInfo, fontSizeInPixels);

    // Example for one character
    char character = 's';
    int width, height, xOffset, yOffset;
    unsigned char* bitmap = stbtt_GetCodepointBitmap(&fontInfo, 0, scale, character, &width, &height, &xOffset, &yOffset);
    
    delete bitmap;

    delete [] buffer;
}
}   // namespace snowpulse
