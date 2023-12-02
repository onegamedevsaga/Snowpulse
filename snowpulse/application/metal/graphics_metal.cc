#include "graphics_metal.h"

#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION

#include <Metal/Metal.hpp>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>
#include <simd/simd.h>

#include "../application.h"
#include "../directory.h"
#include "render_batch_data_metal.h"
#include "render_batch_group_metal.h"

namespace snowpulse {

const char* kSpriteDefault = "sprites/sprite_default.png";

const char* shaderSrc = R"(
    #include <metal_stdlib>
    using namespace metal;

    layout(location = 0) in vec3 position;
    layout(location = 1) in vec2 texCoord;
    layout(location = 2) in vec4 color;
    layout(location = 3) in mat4 transform;

    out vec2 TexCoord;
    out vec4 Color;

    uniform mat4 projection;
    uniform mat4 view;

    struct Uniforms {
        float4x4 projection;
        float4x4 view;
    };

    struct VertexOut
    {
        //float4 position [[position]];
        float2 texCoord [[texCoord]];
        half3 color;
    };

    v2f vertex vertexMain(  uint vertexId [[vertex_id]],
                            device const float3* positions [[buffer(0)]],
                            device const float2* uv [[buffer(1)]],
                            device const float3* colors [[buffer(2)]],
                            device const float4x4* transform [[buffer(3)]],
                            constant Uniforms& uniforms [[buffer(4)]] )
    {
        VertexOut o;
        o.position = float4( positions[ vertexId ], 1.0 );
        o.color = half3 ( colors[ vertexId ] );
        return o;
    }

    half4 fragment fragmentMain( VertexOut in [[stage_in]] )
    {
        return half4( in.color, 1.0 );
    }
)";
std::shared_ptr<GraphicsMetal> GraphicsMetal::Create() {
    auto graphics = new GraphicsMetal();
    return std::shared_ptr<GraphicsMetal>(graphics);
}

GraphicsMetal::GraphicsMetal() {
}

bool GraphicsMetal::Initialize(const Vector2Int& resolution, const Vector2Int& screenSize, void* view) {
    camera_ = Camera::Create();
    renderQueue_ = RenderQueueMetal::Create();
    view_ = static_cast<MTK::View*>(view);
    view_->setColorPixelFormat(MTL::PixelFormatBGRA8Unorm_sRGB);
    device_ = view_->device();

    
    auto ortho = glm::ortho(0.0f, (float)resolution.x, 0.0f, (float)resolution.y, -100.0f, 100.0f);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            projectionMatrix_.columns[i][j] = ortho[i][j];
        }
    }

    BuildShaders();
    BuildBuffers();
    commandQueue_ = device_->newCommandQueue();

    

    // Compile and link shaders
    /*GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
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
*/
    return true;
}

void GraphicsMetal::Shutdown() {
    for (auto const& t : textures_) {
        //glDeleteTextures(1, &(t.second));
    }
    textures_.clear();
    textureSizes_.clear();
#ifdef SPDEBUG
    std::cout << "GraphicsMetal shutdown." << std::endl;
#endif
    
    vertexPositionsBuffer_->release();
    vertexUVsBuffer_->release();
    vertexColorsBuffer_->release();
    transformBuffer_->release();
    uniformsBuffer_->release();
    renderPipelineState_->release();
    commandQueue_->release();
    device_->release();
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
    /*auto fullFilename = GetTextureFullFilename(filename.c_str(), filtering);
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
    }*/
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
            //glDeleteTextures(1, &(i->second));
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
    /*glm::mat4 transform;
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
    }*/
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

void GraphicsMetal::Draw() {
    NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();

    MTL::CommandBuffer* cmd = commandQueue_->commandBuffer();
    MTL::RenderPassDescriptor* pRpd = view_->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder* pEnc = cmd->renderCommandEncoder( pRpd );

    pEnc->setRenderPipelineState(renderPipelineState_);
    pEnc->setVertexBuffer(vertexPositionsBuffer_, 0, 0);
    pEnc->setVertexBuffer(vertexUVsBuffer_, 0, 1);
    pEnc->setVertexBuffer(vertexColorsBuffer_, 0, 2);
    pEnc->setVertexBuffer(transformBuffer_, 0, 3);
    pEnc->setVertexBuffer(uniformsBuffer_, 0, 4);
    pEnc->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3));

    pEnc->endEncoding();
    cmd->presentDrawable( view_->currentDrawable() );
    cmd->commit();

    pPool->release();
}

void GraphicsMetal::BuildShaders() {
    using NS::StringEncoding::UTF8StringEncoding;

    const char* shaderSrc = R"(
        #include <metal_stdlib>
        using namespace metal;

        struct Uniforms {
            float4x4 projection;
            float4x4 view;
        };

        struct VertexOut
        {
            float4 position [[position]];
            float2 texCoord;
            half4 color;
        };

        VertexOut vertex vertexMain(    uint vertexId [[vertex_id]],
                                        device const float3* positions [[buffer(0)]],
                                        device const float2* uv [[buffer(1)]],
                                        device const float4* colors [[buffer(2)]],
                                        device const float4x4* transform [[buffer(3)]],
                                        constant Uniforms& uniforms [[buffer(4)]] )
        {
            VertexOut o;
            o.position = uniforms.projection * uniforms.view * transform[0] * float4(positions[vertexId], 1.0);
            o.texCoord = float2(uv[vertexId]);
            o.color = half4(colors[vertexId]);
            return o;
        }

        half4 fragment fragmentMain( VertexOut in [[stage_in]] )
        {
            return half4(in.color);
        }
    )";

    NS::Error* pError = nullptr;
    MTL::Library* pLibrary = device_->newLibrary( NS::String::string(shaderSrc, UTF8StringEncoding), nullptr, &pError );
    if ( !pLibrary )
    {
        __builtin_printf( "%s", pError->localizedDescription()->utf8String() );
        assert( false );
    }

    MTL::Function* pVertexFn = pLibrary->newFunction( NS::String::string("vertexMain", UTF8StringEncoding) );
    MTL::Function* pFragFn = pLibrary->newFunction( NS::String::string("fragmentMain", UTF8StringEncoding) );

    MTL::RenderPipelineDescriptor* pDesc = MTL::RenderPipelineDescriptor::alloc()->init();
    pDesc->setVertexFunction( pVertexFn );
    pDesc->setFragmentFunction( pFragFn );
    pDesc->colorAttachments()->object(0)->setPixelFormat( view_->colorPixelFormat() );

    renderPipelineState_ = device_->newRenderPipelineState( pDesc, &pError );
    if ( !renderPipelineState_ )
    {
        __builtin_printf( "%s", pError->localizedDescription()->utf8String() );
        assert( false );
    }

    pVertexFn->release();
    pFragFn->release();
    pDesc->release();
    pLibrary->release();
}

struct Uniforms {
    simd::float4x4 projection;
    simd::float4x4 view;
};

void GraphicsMetal::BuildBuffers() {
    const size_t numVertices = 3;

    simd::float3 positions[numVertices] =
    {
        { -200.8f,  200.8f, 0.0f },
        {  0.0f, -200.8f, 0.0f },
        {  200.8f,  200.8f, 0.0f }
    };
    
    for (auto &p : positions) {
        p.x += 400.0f;
        p.y += 800.0f;
    }

    simd::float2 uvs[numVertices] =
    {
        { 0.0f,  0.0f },
        { 0.0f,  0.0f },
        { 0.0f,  0.0f }
    };

    simd::float4 colors[numVertices] =
    {
        { 0.0f, 1.0f, 0.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f, 1.0f }
    };
    
    simd::float4x4 transform = simd::float4x4(1.0f);
    simd::float4x4 view = simd::float4x4(1.0f);

    Matrix4x4 cameraView = camera_->GetMatrix();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            view.columns[i][j] = cameraView.data[j][i];
        }
    }


    

    const size_t positionsDataSize = numVertices * sizeof(simd::float3);
    const size_t uvsDataSize = numVertices * sizeof(simd::float2);
    const size_t colorDataSize = numVertices * sizeof(simd::float4);
    const size_t transformDataSize = sizeof(simd::float4x4);

    vertexPositionsBuffer_ = device_->newBuffer(positionsDataSize, MTL::StorageModeShared);
    vertexUVsBuffer_ = device_->newBuffer(uvsDataSize, MTL::StorageModeShared);
    vertexColorsBuffer_ = device_->newBuffer(colorDataSize, MTL::StorageModeShared);
    transformBuffer_ = device_->newBuffer(transformDataSize, MTL::StorageModeShared);

    Uniforms uniforms;
    uniforms.projection = projectionMatrix_;
    uniforms.view = view;
    uniformsBuffer_ = device_->newBuffer(sizeof(Uniforms), MTL::StorageModeShared);

    memcpy(vertexPositionsBuffer_->contents(), positions, positionsDataSize);
    memcpy(vertexUVsBuffer_->contents(), uvs, uvsDataSize);
    memcpy(vertexColorsBuffer_->contents(), colors, colorDataSize);
    memcpy(transformBuffer_->contents(), &transform, transformDataSize);
    memcpy(uniformsBuffer_->contents(), &uniforms, sizeof(Uniforms));

    vertexPositionsBuffer_->didModifyRange(NS::Range::Make(0, vertexPositionsBuffer_->length()));
    vertexUVsBuffer_->didModifyRange(NS::Range::Make(0, vertexUVsBuffer_->length()));
    vertexColorsBuffer_->didModifyRange(NS::Range::Make(0, vertexColorsBuffer_->length()));
    transformBuffer_->didModifyRange(NS::Range::Make(0, transformBuffer_->length()));
    uniformsBuffer_->didModifyRange(NS::Range::Make(0, uniformsBuffer_->length()));
}
}   // namespace snowpulse
