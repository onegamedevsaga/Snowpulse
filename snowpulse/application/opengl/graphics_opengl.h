#ifndef SNOWPULSE_APPLICATION_OPENGL_GRAPHICSOPENGL_H_
#define SNOWPULSE_APPLICATION_OPENGL_GRAPHICSOPENGL_H_

#include "../../defines.h"
#include "../../game/graphics.h"

#include <memory>
#include <string>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "../../common/vertex.h"
#include "../../common/vector2.h"
#include "../../common/vector2int.h"
#include "../../common/color.h"
#include "render_queue_opengl.h"
#include "render_batch_group_opengl.h"

namespace snowpulse {

class SNOWPULSEAPI GraphicsOpenGL : public Graphics {
    public:
        static std::shared_ptr<GraphicsOpenGL> Create();

        ~GraphicsOpenGL();
        void Initialize(Vector2Int resolution, Camera* camera, RenderQueueOpenGL* renderQueue, Vector2Int screenSize = Vector2Int());
        void Shutdown();

        GLFWwindow* GetWindow() { return window_; }
        Vector2Int GetResolution() { return resolution_; }
        Vector2Int GetScreenSize() { return screenSize_; }

        // Graphics
        Matrix4x4 InvertMatrixNatively(Matrix4x4 matrix);
        void LoadTexture(std::string filename, TextureFiltering filtering, bool rawPath);
        void UnloadTexture(std::string filename, TextureFiltering filtering);
        Vector2 GetTextureSize(std::string filename, TextureFiltering filtering);
        int CreateRenderBatchGroup(int sortOrder);
        void ClearRenderBatchGroups();
        void SubmitRenderBatchGroup(int batchGroup);
        void DrawMesh(Vertex* vertices, unsigned int vertexCount, unsigned short* indices, unsigned int indexCount, std::string textureFullFilename, int sortOrder, BlendMode blendMode, bool isPremultiplied, Matrix4x4 transformMatrix, int batchGroup);
        void DrawSprite(Vector2 size, std::string textureFullFilename, Matrix4x4 transformMatrix, Color color, int sortOrder, BlendMode blendMode, bool isPremultiplied, Vector2 uvLowerLeft, Vector2 uvUpperRight, int batchGroup);

    private:
        GraphicsOpenGL();

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
        RenderQueueOpenGL* renderQueue_;
        std::vector<std::shared_ptr<RenderBatchGroupOpenGL>> renderBatchGroups_;
        std::map<std::string, unsigned int> textures_;
        std::map<unsigned int, Vector2> textureSizes_;
};
}   // namespace snowpulse
#endif
