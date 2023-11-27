#include "graphics_metal.h"

#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION

#include "../application.h"
#include "../directory.h"
#include "render_batch_data_metal.h"
#include "render_batch_group_metal.h"

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


std::shared_ptr<GraphicsMetal> GraphicsMetal::Create() {
    auto graphics = new GraphicsMetal();
    return std::shared_ptr<GraphicsMetal>(graphics);
}

GraphicsMetal::GraphicsMetal() {
}

bool GraphicsMetal::Initialize(const Vector2Int& resolution, const Vector2Int& screenSize) {
    camera_ = Camera::Create();
    renderQueue_ = RenderQueueMetal::Create();

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

    renderQueue_ = RenderQueueMetal::Create();

    LoadTexture(kSpriteDefault, TextureFiltering::kPoint, PathType::kDefaults);

#ifdef SPDEBUG
    std::cout << "GraphicsMetal initialized." << std::endl;
#endif

    return true;
}

void GraphicsMetal::Shutdown() {
    for (auto const& t : textures_) {
        glDeleteTextures(1, &(t.second));
    }
    textures_.clear();
    textureSizes_.clear();
#ifdef SPDEBUG
    std::cout << "GraphicsMetal shutdown." << std::endl;
#endif
}

GraphicsMetal::~GraphicsMetal() {
#ifdef SPDEBUG
    std::cout << "GraphicsMetal destroyed." << std::endl;
#endif
}

Matrix4x4 GraphicsMetal::InvertMatrixNatively(Matrix4x4 matrix) {
    glm::mat4 transform;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            transform[i][j] = matrix.data[i][j];
        }
    }
    transform = glm::inverse(transform);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            matrix.data[i][j] = transform[i][j];
        }
    }
    return matrix;
}

void GraphicsMetal::LoadTexture(std::string filename, TextureFiltering filtering, PathType pathType) {
    auto fullFilename = GetTextureFullFilename(filename.c_str(), filtering);
    switch (pathType) {
        case PathType::kAssets:
            filename = Directory::GetInstance()->GetAssetsPath(filename);
            break;
        case PathType::kDefaults:
            filename = Directory::GetInstance()->GetDefaultsPath(filename);
            break;
        case PathType::kApplicationSupport:
            filename = Directory::GetInstance()->GetApplicationSupportPath(filename);
            break;
        case PathType::kDocuments:
            filename = Directory::GetInstance()->GetDocumentsPath(filename);
            break;
        case PathType::kRaw:
        default:
            break;
    }
    if (!textures_.count(fullFilename)) {
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        switch (filtering) {
            case TextureFiltering::kPoint:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                break;
            case TextureFiltering::kBilinear:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                break;
            case TextureFiltering::kTrilinear:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                break;
            case TextureFiltering::kAnisotropic:
                GLfloat maxAniso = 0.0f;
                glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);
                break;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        int width, height, nrChannels;
        unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 4);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            textures_[fullFilename] = texture;
            textureSizes_[texture] = Vector2(width, height);
        }
        stbi_image_free(data);
    }
}

void GraphicsMetal::UnloadTexture(std::string filename, TextureFiltering filtering) {
    auto fullFilename = GetTextureFullFilename(filename.c_str(), filtering);
    if (!textures_.count(fullFilename)) {
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
    std::cout << "Texture (" << fullFilename.c_str() << ") unloaded." << std::endl;
#endif
}

Vector2 GraphicsMetal::GetTextureSize(std::string filename, TextureFiltering filtering) {
    auto fullFilename = GetTextureFullFilename(filename.c_str(), filtering);
    if (!textures_.count(fullFilename)) {
        return Vector2(100.0f, 100.0f);
    }
    return textureSizes_[textures_[fullFilename]];
}

int GraphicsMetal::CreateRenderBatchGroup(int sortOrder) {
    int id = (int)renderBatchGroups_.size();
    auto group = std::make_shared<RenderBatchGroupMetal>();
    group->sortOrder = sortOrder;
    renderBatchGroups_.push_back(group);
    return id;
}

void GraphicsMetal::ClearRenderBatchGroups() {
    renderBatchGroups_.clear();
}

void GraphicsMetal::SubmitRenderBatchGroup(int batchGroup) {
    if (batchGroup >= 0 && batchGroup < (int)renderBatchGroups_.size()) {
        renderQueue_->Push(renderBatchGroups_[batchGroup]);
    }
}

void GraphicsMetal::DrawMesh(Vertex* vertices, unsigned int vertexCount, unsigned short* indices, unsigned int indexCount, std::string textureFullFilename, int sortOrder, BlendMode blendMode, bool isPremultiplied, Matrix4x4 transformMatrix, int batchGroup) {
    glm::mat4 transform;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            transform[i][j] = transformMatrix.data[i][j];
        }
    }

    auto batch = std::make_shared<RenderBatchDataMetal>();
    batch->drawMode = GL_TRIANGLES;
    batch->shaderProgram = shaderProgram_;
    batch->blendMode = blendMode;
    batch->vertexCount = vertexCount;
    batch->indexCount = indexCount;
    batch->sortOrder = sortOrder;
    batch->isPremultiplied = isPremultiplied;
    batch->texture = textures_[textureFullFilename];
    if (!batch->texture) {
        batch->texture = textures_[GetTextureFullFilename(kSpriteDefault, TextureFiltering::kPoint)];
#ifdef SPDEBUG
        std::cerr << "Can't find texture \"" << textureFullFilename << "\"." << std::endl;
#endif
    }

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
        batch->matrices.push_back(transform);
    }

    auto camMat = camera_->GetMatrix();
    glm::mat4 camMatGLM;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            camMatGLM[i][j] = camMat.data[i][j];
        }
    }

    batch->viewMatrix = camMatGLM;
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

void GraphicsMetal::DrawSprite(Vector2 size, std::string textureFullFilename, Matrix4x4 transformMatrix, Color color, int sortOrder, BlendMode blendMode, bool isPremultiplied, Vector2 uvLowerLeft, Vector2 uvUpperRight, int batchGroup) {
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

    DrawMesh(vertices, 4, indices, 6, textureFullFilename, sortOrder, blendMode, isPremultiplied, transformMatrix, batchGroup);
}
}   // namespace snowpulse
