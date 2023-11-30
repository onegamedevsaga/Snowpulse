#ifndef SNOWPULSE_APPLICATION_METAL_GRAPHICSMETAL_H_
#define SNOWPULSE_APPLICATION_METAL_GRAPHICSMETAL_H_

#include "../../defines.h"
#include "../../game/graphics.h"

#include <memory>
#include <string>
#include <map>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
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
        void LoadTexture(std::string filename, TextureFiltering filtering, PathType pathType) override;
        void UnloadTexture(std::string filename, TextureFiltering filtering) override;
        Vector2 GetTextureSize(std::string filename, TextureFiltering filtering) override;
        int CreateRenderBatchGroup(int sortOrder) override;
        void ClearRenderBatchGroups() override;
        void SubmitRenderBatchGroup(int batchGroup) override;
        void DrawMesh(Vertex* vertices, unsigned int vertexCount, unsigned short* indices, unsigned int indexCount, std::string textureFullFilename, int sortOrder, BlendMode blendMode, bool isPremultiplied, Matrix4x4 transformMatrix, int batchGroup) override;
        void DrawSprite(Vector2 size, std::string textureFullFilename, Matrix4x4 transformMatrix, Color color, int sortOrder, BlendMode blendMode, bool isPremultiplied, Vector2 uvLowerLeft, Vector2 uvUpperRight, int batchGroup) override;

    private:
        GraphicsMetal();

        /*GLuint shaderProgram_;
        glm::mat4 projectionMatrix_;
        GLuint vertexShaderLocation_;
        GLuint uVShaderLocation_;
        GLuint colorShaderLocation_;
        GLuint transformMatrixShaderLocation_;
        GLuint projectionMatrixShaderLocation_;
        GLuint viewMatrixShaderLocation_;*/
        std::shared_ptr<RenderQueueMetal> renderQueue_;
        std::vector<std::shared_ptr<RenderBatchGroupMetal>> renderBatchGroups_;
        std::map<std::string, unsigned int> textures_;
        std::map<unsigned int, Vector2> textureSizes_;
    
        MTK::View* view_;
        MTL::Device* device_;
        MTL::CommandQueue* commandQueue_;
        MTL::RenderPipelineState* renderPipelineState_;
        MTL::Buffer* vertexPositionsBuffer_;
        MTL::Buffer* vertexColorsBuffer_;
};
}   // namespace snowpulse
#endif
