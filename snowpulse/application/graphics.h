#ifndef SNOWPULSE_APPLICATION_GRAPHICS_H_
#define SNOWPULSE_APPLICATION_GRAPHICS_H_

#include "../defines.h"

#include <memory>
#include <string>

#include "camera.h"
#include "../common/vertex.h"
#include "../common/vector2.h"
#include "../common/vector2int.h"
#include "../common/blend_mode.h"
#include "../common/color.h"
#include "../common/matrix4x4.h"
#include "../common/texture_filtering.h"
#include "../common/path_type.h"

namespace snowpulse {

class SNOWPULSEAPI Graphics {
    public:
        virtual ~Graphics() = default;
        virtual void UpdateProjectionMatrix(const Vector2Int& resolution) = 0;
        virtual void Shutdown() = 0;
        virtual Matrix4x4 InvertMatrixNatively(Matrix4x4 matrix) = 0;
        virtual bool LoadTexture(std::string filename, PathType pathType = PathType::kAssets) = 0;
        virtual void LoadTexture(std::string name, unsigned char* bitmap, Vector2Int size) = 0;
        virtual void UnloadTexture(std::string filename) = 0;
        virtual Vector2 GetTextureSize(std::string filename) = 0;
        virtual int CreateRenderBatchGroup(int sortOrder) = 0;
        virtual void ClearRenderBatchGroups() = 0;
        virtual void SubmitRenderBatchGroup(int batchGroup) = 0;
        virtual void DrawMesh(Vertex* vertices, unsigned int vertexCount, unsigned short* indices, unsigned int indexCount, std::string textureFilename, int sortOrder, BlendMode blendMode, TextureFiltering filtering, bool isPremultiplied, Matrix4x4 transformMatrix, int batchGroup = -1) = 0;
        virtual void DrawSprite(Vector2 size, std::string textureFilename, Matrix4x4 transformMatrix, Color color = Color::White(), int sortOrder = 0, BlendMode blendMode = BlendMode::kNormal, TextureFiltering filtering = TextureFiltering::kBilinear, bool isPremultiplied = false, Vector2 uvLowerLeft = Vector2(0.0f, 1.0f), Vector2 uvUpperRight = Vector2(1.0f, 0.0f), int batchGroup = -1) = 0;

        Camera* GetCamera() const { return camera_.get(); }
        bool IsDepthTesting() const { return isDepthTesting_; }

        void SetDepthTesting(bool value) { isDepthTesting_ = value; }

    protected:
        std::shared_ptr<Camera> camera_;
        bool isDepthTesting_ = false;
};
}   // namespace snowpulse

#endif
