#define STB_IMAGE_IMPLEMENTATION

#include "graphics_opengl.h"

#include <iostream>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../application.h"
#include "../directory.h"
#include "render_batch_data_opengl.h"
#include "render_batch_group_opengl.h"

namespace snowpulse {

const char* kSpriteDefault = "sprites/sprite_default.png";

// Vertex shader source
const char* kVertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec2 texCoord;
    layout(location = 2) in vec4 color;
    layout(location = 3) in mat4 transform;

    out vec2 TexCoord;
    out vec4 Color;

    uniform mat4 projection;
    uniform mat4 view;

    void main() {
        TexCoord = texCoord;
        Color = color;
        gl_Position = projection * view * transform * vec4(position, 1.0f);
    }
)";

// Fragment shader source
const char* kFragmentShaderSource = R"(
    #version 330 core
    in vec2 TexCoord;
    in vec4 Color;
    out vec4 FragColor;

    uniform sampler2D spriteTexture;

    void main() {
        FragColor = Color * texture(spriteTexture, TexCoord);
    }
)";

std::shared_ptr<GraphicsOpenGL> GraphicsOpenGL::Create() {
    auto graphics = new GraphicsOpenGL();
    return std::shared_ptr<GraphicsOpenGL>(graphics);
}

GraphicsOpenGL::GraphicsOpenGL() {
}

bool GraphicsOpenGL::Initialize(const Vector2Int& resolution, const Vector2Int& screenSize) {
    camera_ = Camera::Create(resolution);
    renderQueue_ = RenderQueueOpenGL::Create();

    // Compile and link shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &kVertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &kFragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram_ = glCreateProgram();
    glAttachShader(shaderProgram_, vertexShader);
    glAttachShader(shaderProgram_, fragmentShader);
    glLinkProgram(shaderProgram_);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    projectionMatrix_ = glm::ortho(0.0f, (float)resolution.x, 0.0f, (float)resolution.y, -100.0f, 100.0f);

    vertexShaderLocation_ = glGetAttribLocation(shaderProgram_, "position");
    uVShaderLocation_ = glGetAttribLocation(shaderProgram_, "texCoord");
    colorShaderLocation_ = glGetAttribLocation(shaderProgram_, "color");
    transformMatrixShaderLocation_ = glGetAttribLocation(shaderProgram_, "transform");
    projectionMatrixShaderLocation_ = glGetUniformLocation(shaderProgram_, "projection");
    viewMatrixShaderLocation_ = glGetUniformLocation(shaderProgram_, "view");

    if (isDepthTesting_) {
        glEnable(GL_DEPTH_TEST);
    }

    UpdateProjectionMatrix(resolution);
    LoadTexture(kSpriteDefault, PathType::kDefaults);

    #ifdef SPDEBUG
    std::cout << "GraphicsMetal initialized." << std::endl;
    #endif

    return true;
}

void GraphicsOpenGL::UpdateProjectionMatrix(const Vector2Int& resolution) {
    auto ortho = glm::ortho(0.0f, (float)resolution.x, 0.0f, (float)resolution.y, -100.0f, 100.0f);
    memcpy(glm::value_ptr(projectionMatrix_), glm::value_ptr(ortho), sizeof(projectionMatrix_));
}

void GraphicsOpenGL::Shutdown() {
    for (auto const& t : textures_) {
        glDeleteTextures(1, &(t.second));
    }
    textures_.clear();
    textureSizes_.clear();
#ifdef SPDEBUG
    std::cout << "GraphicsOpenGL shutdown." << std::endl;
#endif
}

GraphicsOpenGL::~GraphicsOpenGL() {
#ifdef SPDEBUG
    std::cout << "GraphicsOpenGL destroyed." << std::endl;
#endif
}

Matrix4x4 GraphicsOpenGL::InvertMatrixNatively(Matrix4x4 matrix) {
    glm::mat4 transform;
    memcpy(glm::value_ptr(transform), matrix.data, sizeof(matrix.data));
    transform = glm::inverse(transform);
    memcpy(matrix.data, glm::value_ptr(transform), sizeof(matrix.data));
    return matrix;
}

void GraphicsOpenGL::LoadTexture(std::string filename, PathType pathType) {
    std::string fullFilename = Directory::GetInstance()->GetFullFilename(filename, pathType);
    if (!textures_.count(filename)) {
        int width, height, nrChannels;
        unsigned char *data = stbi_load(fullFilename.c_str(), &width, &height, &nrChannels, 4);
        if (data) {
            LoadTexture(filename, data, Vector2Int(width, height));
        }
        stbi_image_free(data);
    }
}

void GraphicsOpenGL::LoadTexture(std::string name, unsigned char* bitmap, Vector2Int size) {
    auto width = size.x;
    auto height = size.y;
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
    glGenerateMipmap(GL_TEXTURE_2D);
    textures_[name] = texture;
    textureSizes_[texture] = Vector2(width, height);
}

void GraphicsOpenGL::UnloadTexture(std::string filename) {
    if (!textures_.count(filename)) {
        return;
    }
    for (auto i = textures_.begin(); i != textures_.end(); i++) {
        if (i->first == filename) {
            for (auto i2 = textureSizes_.begin(); i2 != textureSizes_.end(); i2++) {
                if (i2->first == i->second) {
                    textureSizes_.erase(i2);
                    break;
                }
            }
            glDeleteTextures(1, &(i->second));
            textures_.erase(i);
            break;
        }
    }
#ifdef SPDEBUG
    std::cout << "Texture (" << filename.c_str() << ") unloaded." << std::endl;
#endif
}

Vector2 GraphicsOpenGL::GetTextureSize(std::string filename) {
    if (!textures_.count(filename)) {
        return Vector2(100.0f, 100.0f);
    }
    return textureSizes_[textures_[filename]];
}

int GraphicsOpenGL::CreateRenderBatchGroup(int sortOrder) {
    int id = (int)renderBatchGroups_.size();
    auto group = std::make_shared<RenderBatchGroupOpenGL>();
    group->sortOrder = sortOrder;
    renderBatchGroups_.push_back(group);
    return id;
}

void GraphicsOpenGL::ClearRenderBatchGroups() {
    renderBatchGroups_.clear();
}

void GraphicsOpenGL::SubmitRenderBatchGroup(int batchGroup) {
    if (batchGroup >= 0 && batchGroup < (int)renderBatchGroups_.size()) {
        renderQueue_->Push(renderBatchGroups_[batchGroup]);
    }
}

void GraphicsOpenGL::DrawMesh(Vertex* vertices, unsigned int vertexCount, unsigned short* indices, unsigned int indexCount, std::string textureFilename, int sortOrder, BlendMode blendMode, TextureFiltering filtering, bool isPremultiplied, Matrix4x4 transformMatrix, int batchGroup) {
    auto batch = std::make_shared<RenderBatchDataOpenGL>();
    batch->drawMode = GL_TRIANGLES;
    batch->shaderProgram = shaderProgram_;
    batch->blendMode = blendMode;
    batch->textureFiltering = filtering;
    batch->vertexCount = vertexCount;
    batch->indexCount = indexCount;
    batch->sortOrder = sortOrder;
    batch->isPremultiplied = isPremultiplied;
    if (textures_.count(textureFilename)) {
        batch->texture = textures_[textureFilename];
    }
    else {
        batch->texture = textures_[kSpriteDefault];
#ifdef SPDEBUG
        std::cerr << "Error: Graphics: Can't find texture \"" << textureFilename << "\"." << std::endl;
#endif
    }

    glm::mat4 transMat;
    memcpy(glm::value_ptr(transMat), transformMatrix.data, sizeof(transMat));

    batch->vertices.reserve(batch->vertexCount * 9);
    batch->indices.reserve(batch->indexCount);
    batch->matrices.reserve(batch->vertexCount);

    for (int i = 0; i < batch->vertexCount; i++) {
        batch->vertices.push_back(vertices[i].position.x);
        batch->vertices.push_back(vertices[i].position.y);
        batch->vertices.push_back(vertices[i].position.z);
        batch->vertices.push_back(vertices[i].uv.x);
        batch->vertices.push_back(vertices[i].uv.y);
        batch->vertices.push_back(vertices[i].color.r);
        batch->vertices.push_back(vertices[i].color.g);
        batch->vertices.push_back(vertices[i].color.b);
        batch->vertices.push_back(vertices[i].color.a);
    }

    for (int i = 0; i < batch->indexCount; i++) {
        batch->indices.push_back(indices[i]);
    }

    for (int i = 0; i < batch->vertexCount; i++) {
        batch->matrices.push_back(transMat);
    }

    auto cameraView = camera_->GetMatrix();
    memcpy(glm::value_ptr(batch->viewMatrix), cameraView.data, sizeof(batch->viewMatrix));

    batch->projectionMatrix = projectionMatrix_;
    batch->vertexShaderLocation = vertexShaderLocation_;
    batch->uVShaderLocation = uVShaderLocation_;
    batch->colorShaderLocation = colorShaderLocation_;
    batch->transformMatrixShaderLocation = transformMatrixShaderLocation_;
    batch->viewMatrixShaderLocation = viewMatrixShaderLocation_;
    batch->projectionMatrixShaderLocation = projectionMatrixShaderLocation_;

    if (batchGroup >= 0 && batchGroup < (int)renderBatchGroups_.size()) {
        auto group = renderBatchGroups_[batchGroup];
        group->AddBatch(batch);
    }
    else {
        renderQueue_->Push(batch);
    }
}

void GraphicsOpenGL::DrawSprite(Vector2 size, std::string textureFilename, Matrix4x4 transformMatrix, Color color, int sortOrder, BlendMode blendMode, TextureFiltering filtering, bool isPremultiplied, Vector2 uvLowerLeft, Vector2 uvUpperRight, int batchGroup) {
    float halfWidth = size.x * 0.5f;
    float halfHeight = size.y * 0.5f;

    Vertex vertices[4];
    vertices[0].position = Vector3( halfWidth,  halfHeight, 0.0f);
    vertices[1].position = Vector3( halfWidth, -halfHeight, 0.0f);
    vertices[2].position = Vector3(-halfWidth, -halfHeight, 0.0f);
    vertices[3].position = Vector3(-halfWidth,  halfHeight, 0.0f);
    vertices[0].uv = Vector2(uvUpperRight.x, uvUpperRight.y);
    vertices[1].uv = Vector2(uvUpperRight.x, uvLowerLeft.y);
    vertices[2].uv = Vector2(uvLowerLeft.x,  uvLowerLeft.y);
    vertices[3].uv = Vector2(uvLowerLeft.x,  uvUpperRight.y);
    vertices[0].color = color;
    vertices[1].color = color;
    vertices[2].color = color;
    vertices[3].color = color;

    unsigned short indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    DrawMesh(vertices, 4, indices, 6, textureFilename, sortOrder, blendMode, filtering, isPremultiplied, transformMatrix, batchGroup);
}
}   // namespace snowpulse
