#ifndef SNOWPULSE_APPLICATION_METAL_GRAPHICSMETAL_H_
#define SNOWPULSE_APPLICATION_METAL_GRAPHICSMETAL_H_

#include "../../defines.h"
#include "../../game/graphics.h"

#include <memory>
#include <string>
#include <map>
#include <simd/simd.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#include "../../common/vertex.h"
#include "../../common/vector2.h"
#include "../../common/vector2int.h"
#include "../../common/color.h"
#include "render_queue_metal.h"
#include "render_batch_group_metal.h"

namespace MTK {
class View;
}

namespace MTL {
class Device;
class CommandQueue;
class RenderPipelineState;
class Buffer;
class Texture;
}

namespace snowpulse {

class SNOWPULSEAPI GraphicsMetal : public Graphics {
    public:
        static std::shared_ptr<GraphicsMetal> Create();

        ~GraphicsMetal();
        bool Initialize(const Vector2Int& resolution, const Vector2Int& screenSize, void* view);
        void Shutdown() override;

        RenderQueueMetal* GetRenderQueue() const { return renderQueue_.get(); }

        // Graphics
        Matrix4x4 InvertMatrixNatively(Matrix4x4 matrix) override;
        void LoadTexture(std::string filename, PathType pathType) override;
        void UnloadTexture(std::string filename) override;
        Vector2 GetTextureSize(std::string filename) override;
        int CreateRenderBatchGroup(int sortOrder) override;
        void ClearRenderBatchGroups() override;
        void SubmitRenderBatchGroup(int batchGroup) override;
        void DrawMesh(Vertex* vertices, unsigned int vertexCount, unsigned short* indices, unsigned int indexCount, std::string filename, int sortOrder, BlendMode blendMode, TextureFiltering filtering, bool isPremultiplied, Matrix4x4 transformMatrix, int batchGroup) override;
        void DrawSprite(Vector2 size, std::string filename, Matrix4x4 transformMatrix, Color color, int sortOrder, BlendMode blendMode, TextureFiltering filtering, bool isPremultiplied, Vector2 uvLowerLeft, Vector2 uvUpperRight, int batchGroup) override;
    
        void Draw();

    private:
        GraphicsMetal();
        void BuildShaders();
        void BuildBuffers();

        /*GLuint shaderProgram_;
        glm::mat4 projectionMatrix_;
        GLuint vertexShaderLocation_;
        GLuint uVShaderLocation_;
        GLuint colorShaderLocation_;
        GLuint transformMatrixShaderLocation_;
        GLuint projectionMatrixShaderLocation_;
        GLuint viewMatrixShaderLocation_;*/
        
        
        
        

        MTK::View* view_;
        MTL::Device* device_;
        MTL::CommandQueue* commandQueue_;
        MTL::RenderPipelineState* renderPipelineState_;
        MTL::Buffer* indexBuffer_;
        MTL::Buffer* vertexPositionsBuffer_;
        MTL::Buffer* vertexUVsBuffer_;
        MTL::Buffer* vertexColorsBuffer_;
        MTL::Buffer* transformBuffer_;
        MTL::Buffer* uniformsBuffer_;

        simd::float4x4 projectionMatrix_;
        std::map<std::string, MTL::Texture*> textures_;
        std::map<MTL::Texture*, Vector2> textureSizes_;
        std::shared_ptr<RenderQueueMetal> renderQueue_;
        std::vector<std::shared_ptr<RenderBatchGroupMetal>> renderBatchGroups_;
};
}   // namespace snowpulse
#endif
