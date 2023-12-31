#ifndef SNOWPULSE_APPLICATION_OPENGL_RENDERBATCHDATAOPENGL_H_
#define SNOWPULSE_APPLICATION_OPENGL_RENDERBATCHDATAOPENGL_H_

#include "../../defines.h"
#include "../render_batch.h"

#include <memory>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../../common/blend_mode.h"
#include "../../common/texture_filtering.h"

namespace snowpulse {
class SNOWPULSEAPI RenderBatchDataOpenGL : public RenderBatch {
    public:
        RenderBatchDataOpenGL() : RenderBatch(RenderBatchType::kData) { }
        ~RenderBatchDataOpenGL() { }

        bool CanMergeWith(const RenderBatchDataOpenGL* other) const {
            return  drawMode == other->drawMode &&
                    blendMode == other->blendMode &&
                    textureFiltering == other->textureFiltering &&
                    isPremultiplied == other->isPremultiplied &&
                    viewMatrix == other->viewMatrix &&
                    projectionMatrix == other->projectionMatrix &&
                    shaderProgram == other->shaderProgram &&
                    hasTexture == other->hasTexture &&
                    (!hasTexture || texture == other->texture);
        }

        unsigned int vertexPlusUVPlusColorCount = 9;
        unsigned int vertexSectionCount = 3;
        unsigned int uVSectionCount = 2;
        unsigned int colorSectionCount = 4;

        GLenum drawMode;
        GLuint shaderProgram;
        BlendMode blendMode;
        TextureFiltering textureFiltering;
        GLsizei vertexCount;
        GLsizei indexCount;

        std::vector<float> vertices;
        std::vector<unsigned short> indices;
        std::vector<glm::mat4> matrices;

        GLuint vertexShaderLocation;
        GLuint uVShaderLocation;
        GLuint transformMatrixShaderLocation;
        GLuint colorShaderLocation;
        GLuint viewMatrixShaderLocation;
        GLuint projectionMatrixShaderLocation;

        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;

        bool isPremultiplied;
        bool hasTexture;
        GLuint texture;
};
}   // namespace snowpulse
#endif
