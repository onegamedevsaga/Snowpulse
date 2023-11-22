#ifndef SNOWPULSE_GAME_GRAPHICS_H_
#define SNOWPULSE_GAME_GRAPHICS_H_

#include "../defines.h"

#include <memory>
#include <string>

#include "../common/vertex.h"
#include "../common/vector2.h"
#include "../common/blend_mode.h"
#include "../common/color.h"
#include "../common/matrix4x4.h"
#include "../common/texture_filtering.h"
#include "../application/camera.h"
#include "components/transform.h"

namespace snowpulse {

class SNOWPULSEAPI Graphics {
    public:
        virtual ~Graphics() = default;
        virtual void Shutdown() = 0;
        virtual Matrix4x4 InvertMatrixNatively(Matrix4x4 matrix) = 0;
        virtual void LoadTexture(std::string filename, TextureFiltering filtering = TextureFiltering::kBilinear) = 0;
        virtual void UnloadTexture(std::string filename, TextureFiltering filtering) = 0;
        virtual Vector2 GetTextureSize(std::string filename, TextureFiltering filtering) = 0;
        virtual int CreateRenderBatchGroup(int sortOrder) = 0;
        virtual void ClearRenderBatchGroups() = 0;
        virtual void SubmitRenderBatchGroup(int batchGroup) = 0;
        virtual void DrawMesh(Vertex* vertices, unsigned int vertexCount, unsigned short* indices, unsigned int indexCount, std::string textureFilename, int sortOrder, BlendMode blendMode, bool isPremultiplied, Matrix4x4 transformMatrix, int batchGroup = -1) = 0;
        virtual void DrawSprite(Vector2 size, std::string filename, Matrix4x4 transformMatrix, Color color = Color::White(), int sortOrder = 0, BlendMode blendMode = BlendMode::kNormal, bool isPremultiplied = false, Vector2 uvLowerLeft = Vector2(0.0f, 1.0f), Vector2 uvUpperRight = Vector2(1.0f, 0.0f), int batchGroup = -1) = 0;

        Camera* GetCamera() const { return camera_; }
        bool IsDepthTesting() const { return isDepthTesting_; }

        void SetDepthTesting(bool value) { isDepthTesting_ = value; }

        std::string GetTextureFullFilename(std::string filename, TextureFiltering filtering) {
            return filename + "_" + std::to_string(static_cast<int>(filtering));
        }

    protected:
        Camera* camera_;
        bool isDepthTesting_ = false;
};
}   // namespace snowpulse

#endif
