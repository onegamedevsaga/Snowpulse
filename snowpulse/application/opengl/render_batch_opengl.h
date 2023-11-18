#ifndef SNOWPULSE_APPLICATION_OPENGL_RENDERBATCHOPENGL_H_
#define SNOWPULSE_APPLICATION_OPENGL_RENDERBATCHOPENGL_H_

#include "../../defines.h"

#include <memory>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include "../../common/blend_mode.h"

namespace snowpulse {
    class SNOWPULSEAPI RenderBatchOpenGL {
        public:
            int vertexPlusUVPlusColorCount = 9;
            int vertexSectionCount = 3;
            int uVSectionCount = 2;
            int colorSectionCount = 4;
            int sortOrder = 0;

            GLenum drawMode;
            GLuint shaderProgram;
            BlendMode blendMode;
            GLsizei vertexCount;
            GLsizei indexCount;

            std::vector<float> vertices;
            std::vector<int> indices;
            std::vector<glm::mat4> matrices;

            GLuint vertexShaderLocation;
            GLuint uVShaderLocation;
            GLuint transformMatrixShaderLocation;
            GLuint colorShaderLocation;
            GLuint viewMatrixShaderLocation;
            GLuint projectionMatrixShaderLocation;

            glm::mat4 viewMatrix;
            glm::mat4 projectionMatrix;

            GLuint texture;
    };
}   // namespace snowpulse
#endif
