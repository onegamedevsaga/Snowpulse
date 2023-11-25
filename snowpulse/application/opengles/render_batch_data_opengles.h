#ifndef SNOWPULSE_APPLICATION_OPENGLES_RENDERBATCHDATAOPENGLES_H_
#define SNOWPULSE_APPLICATION_OPENGLES_RENDERBATCHDATAOPENGLES_H_

#include "../../defines.h"
#include "../render_batch.h"

#include <memory>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include "../../common/blend_mode.h"

namespace snowpulse {
class SNOWPULSEAPI RenderBatchDataOpenGLES : public RenderBatch {
    public:
        RenderBatchDataOpenGLES() : RenderBatch(RenderBatchType::kData) { }
        ~RenderBatchDataOpenGLES() { }

        unsigned int vertexPlusUVPlusColorCount = 9;
        unsigned int vertexSectionCount = 3;
        unsigned int uVSectionCount = 2;
        unsigned int colorSectionCount = 4;

        GLenum drawMode;
        GLuint shaderProgram;
        BlendMode blendMode;
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
        GLuint texture;
};
}   // namespace snowpulse
#endif
