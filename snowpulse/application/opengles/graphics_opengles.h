#ifndef SNOWPULSE_APPLICATION_OPENGLES_GRAPHICSOPENGLES_H_
#define SNOWPULSE_APPLICATION_OPENGLES_GRAPHICSOPENGLES_H_

#include "../../defines.h"
#include "../../game/graphics.h"

#include <memory>
#include <string>
#include <map>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "../../common/vertex.h"
#include "../../common/vector2.h"
#include "../../common/vector2int.h"
#include "../../common/color.h"
#include "render_queue_opengles.h"
#include "render_batch_group_opengles.h"

namespace snowpulse {

class SNOWPULSEAPI GraphicsOpenGLES : public Graphics {
    public:
        static std::shared_ptr<GraphicsOpenGLES> Create();

        ~GraphicsOpenGLES();
        void Initialize(Vector2Int resolution, Camera* camera, RenderQueueOpenGLES* renderQueue, Vector2Int screenSize = Vector2Int());
        void Shutdown() override;

        GLFWwindow* GetWindow() { return window_; }
        Vector2Int GetResolution() { return resolution_; }
        Vector2Int GetScreenSize() { return screenSize_; }

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
        GraphicsOpenGLES();

        GLFWwindow* window_;
        Vector2Int resolution_;
        Vector2Int screenSize_;
        GLuint shaderProgram_;
        glm::mat4 projectionMatrix_;
        GLuint vertexShaderLocation_;
        GLuint uVShaderLocation_;
        GLuint colorShaderLocation_;
        GLuint transformMatrixShaderLocation_;
        GLuint projectionMatrixShaderLocation_;
        GLuint viewMatrixShaderLocation_;
        RenderQueueOpenGLES* renderQueue_;
        std::vector<std::shared_ptr<RenderBatchGroupOpenGLES>> renderBatchGroups_;
        std::map<std::string, unsigned int> textures_;
        std::map<unsigned int, Vector2> textureSizes_;
};
}   // namespace snowpulse
#endif
