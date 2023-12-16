#ifndef SNOWPULSE_APPLICATION_OPENGL_GRAPHICSOPENGL_H_
#define SNOWPULSE_APPLICATION_OPENGL_GRAPHICSOPENGL_H_

#include "../../defines.h"
#include "../graphics.h"

#include <memory>
#include <string>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

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
        bool Initialize(const Vector2Int& resolution, const Vector2Int& screenSize = Vector2Int());
        void Shutdown() override;

        RenderQueueOpenGL* GetRenderQueue() const { return renderQueue_.get(); }

        // Graphics
        Matrix4x4 InvertMatrixNatively(Matrix4x4 matrix) override;
        void LoadTexture(std::string filename, PathType pathType) override;
        void UnloadTexture(std::string filename) override;
        Vector2 GetTextureSize(std::string filename) override;
        int CreateRenderBatchGroup(int sortOrder) override;
        void ClearRenderBatchGroups() override;
        void SubmitRenderBatchGroup(int batchGroup) override;
        void DrawMesh(Vertex* vertices, unsigned int vertexCount, unsigned short* indices, unsigned int indexCount, std::string textureFilename, int sortOrder, BlendMode blendMode, TextureFiltering filtering, bool isPremultiplied, Matrix4x4 transformMatrix, int batchGroup) override;
        void DrawSprite(Vector2 size, std::string textureFilename, Matrix4x4 transformMatrix, Color color, int sortOrder, BlendMode blendMode, TextureFiltering filtering, bool isPremultiplied, Vector2 uvLowerLeft, Vector2 uvUpperRight, int batchGroup) override;

    private:
        GraphicsOpenGL();

        GLuint shaderProgram_;
        glm::mat4 projectionMatrix_;
        GLuint vertexShaderLocation_;
        GLuint uVShaderLocation_;
        GLuint colorShaderLocation_;
        GLuint transformMatrixShaderLocation_;
        GLuint projectionMatrixShaderLocation_;
        GLuint viewMatrixShaderLocation_;
        std::shared_ptr<RenderQueueOpenGL> renderQueue_;
        std::vector<std::shared_ptr<RenderBatchGroupOpenGL>> renderBatchGroups_;
        std::map<std::string, unsigned int> textures_;
        std::map<unsigned int, Vector2> textureSizes_;
};
}   // namespace snowpulse
#endif
