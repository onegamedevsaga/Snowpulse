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
    view_->setClearColor(MTL::ClearColor::Make(0.2f, 0.3f, 0.3f, 1.0));
    device_ = view_->device();

    
    auto ortho = glm::ortho(0.0f, (float)resolution.x, 0.0f, (float)resolution.y, -100.0f, 100.0f);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            projectionMatrix_.columns[i][j] = ortho[i][j];
        }
    }

    LoadTexture("logo.png", PathType::kAssets);
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
        t.second->release();
    }
    textures_.clear();
    textureSizes_.clear();
#ifdef SPDEBUG
    std::cout << "GraphicsMetal shutdown." << std::endl;
#endif
    
    indexBuffer_->release();
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

void GraphicsMetal::LoadTexture(std::string filename, PathType pathType) {
    std::string fullFilename = "";
    switch (pathType) {
        case PathType::kAssets:
            fullFilename = Directory::GetInstance()->GetAssetsPath(filename);
            break;
        case PathType::kDefaults:
            fullFilename = Directory::GetInstance()->GetDefaultsPath(filename);
            break;
        case PathType::kApplicationSupport:
            fullFilename = Directory::GetInstance()->GetApplicationSupportPath(filename);
            break;
        case PathType::kDocuments:
            fullFilename = Directory::GetInstance()->GetDocumentsPath(filename);
            break;
        case PathType::kRaw:
            fullFilename = filename;
        default:
            break;
    }
    if (!textures_.count(filename)) {
        int width, height, nrChannels;
        unsigned char *data = stbi_load(fullFilename.c_str(), &width, &height, &nrChannels, 4);
        if (data) {
            NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();
            MTL::TextureDescriptor* texDesc = MTL::TextureDescriptor::alloc()->init();
            texDesc->setWidth(width);
            texDesc->setHeight(height);
            texDesc->setPixelFormat(MTL::PixelFormatRGBA8Unorm_sRGB);//(MTL::PixelFormatRGBA8Unorm); // Match the format
            texDesc->setTextureType(MTL::TextureType2D);
            texDesc->setStorageMode(MTL::StorageModeShared);
            texDesc->setUsage(MTL::TextureUsageShaderRead);

            MTL::Texture* texture = device_->newTexture(texDesc);
            MTL::Region region = MTL::Region(0, 0, width, height);
            texture->replaceRegion(region, 0, data, width * 4);

            textures_[filename] = texture;
            textureSizes_[texture] = Vector2(width, height);
            texDesc->release();
            pPool->release();
        }
        stbi_image_free(data);
    }
}

void GraphicsMetal::UnloadTexture(std::string filename) {
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
            i->second->release();
            textures_.erase(i);
            break;
        }
    }
#ifdef SPDEBUG
    std::cout << "Texture (" << filename.c_str() << ") unloaded." << std::endl;
#endif
}

Vector2 GraphicsMetal::GetTextureSize(std::string filename) {
    if (!textures_.count(filename)) {
        return Vector2(100.0f, 100.0f);
    }
    return textureSizes_[textures_[filename]];
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

void GraphicsMetal::DrawMesh(Vertex* vertices, unsigned int vertexCount, unsigned short* indices, unsigned int indexCount, std::string filename, int sortOrder, BlendMode blendMode, TextureFiltering filtering, bool isPremultiplied, Matrix4x4 transformMatrix, int batchGroup) {
    
}

void GraphicsMetal::DrawSprite(Vector2 size, std::string filename, Matrix4x4 transformMatrix, Color color, int sortOrder, BlendMode blendMode, TextureFiltering filtering, bool isPremultiplied, Vector2 uvLowerLeft, Vector2 uvUpperRight, int batchGroup) {
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

    DrawMesh(vertices, 4, indices, 6, filename, sortOrder, blendMode, filtering, isPremultiplied, transformMatrix, batchGroup);
}

void GraphicsMetal::Draw() {
    NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();
    
    MTL::SamplerDescriptor* samplerDesc = MTL::SamplerDescriptor::alloc()->init();
    // Configure sampler descriptor...
    samplerDesc->setMinFilter(MTL::SamplerMinMagFilterLinear);
    samplerDesc->setMagFilter(MTL::SamplerMinMagFilterLinear);
    samplerDesc->setSAddressMode(MTL::SamplerAddressModeClampToEdge);
    samplerDesc->setTAddressMode(MTL::SamplerAddressModeClampToEdge);

    MTL::SamplerState* samplerState = device_->newSamplerState(samplerDesc);
    
    

    MTL::CommandBuffer* cmd = commandQueue_->commandBuffer();
    MTL::RenderPassDescriptor* pRpd = view_->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder* pEnc = cmd->renderCommandEncoder( pRpd );

    pEnc->setRenderPipelineState(renderPipelineState_);
    pEnc->setVertexBuffer(vertexPositionsBuffer_, 0, 0);
    pEnc->setVertexBuffer(vertexUVsBuffer_, 0, 1);
    pEnc->setVertexBuffer(vertexColorsBuffer_, 0, 2);
    pEnc->setVertexBuffer(transformBuffer_, 0, 3);
    pEnc->setVertexBuffer(uniformsBuffer_, 0, 4);
    pEnc->setFragmentTexture(textures_["logo.png"], 0);
    pEnc->setFragmentSamplerState(samplerState, 0);
    pEnc->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger(6), MTL::IndexTypeUInt16, indexBuffer_, NS::UInteger(0), NS::UInteger(1));

    pEnc->endEncoding();
    cmd->presentDrawable( view_->currentDrawable() );
    cmd->commit();

    samplerDesc->release();
    samplerState->release();
    pPool->release();
}

void GraphicsMetal::BuildShaders() {
    using NS::StringEncoding::UTF8StringEncoding;
    
    NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();

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

        half4 fragment fragmentMain(    texture2d<float> texture [[texture(0)]],
                                        sampler s [[sampler(0)]],
                                        VertexOut in [[stage_in]] )
        {
            return half4(texture.sample(s, in.texCoord)) * in.color;
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

    auto colorDesc = pDesc->colorAttachments()->object(0);
    colorDesc->setPixelFormat(view_->colorPixelFormat());
    colorDesc->setBlendingEnabled(true);
    colorDesc->setRgbBlendOperation(MTL::BlendOperationAdd);
    colorDesc->setAlphaBlendOperation(MTL::BlendOperationAdd);
    colorDesc->setSourceRGBBlendFactor(MTL::BlendFactorOne);
    colorDesc->setSourceAlphaBlendFactor(MTL::BlendFactorOne);
    colorDesc->setDestinationRGBBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
    colorDesc->setDestinationAlphaBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);

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
    pPool->release();
}

struct Uniforms {
    simd::float4x4 projection;
    simd::float4x4 view;
};

void GraphicsMetal::BuildBuffers() {
    NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();
    const size_t numVertices = 4;
    float halfWidth = 900.0f;
    float halfHeight = 900.0f;

    simd::float3 positions[numVertices] =
    {
        {  halfWidth,  halfHeight, 0.0f },
        {  halfWidth, -halfHeight, 0.0f },
        { -halfWidth, -halfHeight, 0.0f },
        { -halfWidth,  halfHeight, 0.0f }
    };

    simd::float2 uvs[numVertices] =
    {
        { 1.0f,  0.0f },
        { 1.0f,  1.0f },
        { 0.0f,  1.0f },
        { 0.0f,  0.0f }
    };

    simd::float4 colors[numVertices] =
    {
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f }
    };
    
    simd::float4x4 transform = simd::float4x4(1.0f);
    simd::float4x4 view = simd::float4x4(1.0f);

    camera_->SetPosition(Vector2(-1242.0f * 0.5f, -2688.0f * 0.5f));
    Matrix4x4 cameraView = camera_->GetMatrix();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            view.columns[i][j] = cameraView.data[i][j];
        }
    }
    
    uint16_t indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    const size_t indicesDataSize = 6 * sizeof(uint16_t);
    const size_t positionsDataSize = numVertices * sizeof(simd::float3);
    const size_t uvsDataSize = numVertices * sizeof(simd::float2);
    const size_t colorDataSize = numVertices * sizeof(simd::float4);
    const size_t transformDataSize = sizeof(simd::float4x4);

    indexBuffer_ = device_->newBuffer(indicesDataSize, MTL::StorageModeShared);
    vertexPositionsBuffer_ = device_->newBuffer(positionsDataSize, MTL::StorageModeShared);
    vertexUVsBuffer_ = device_->newBuffer(uvsDataSize, MTL::StorageModeShared);
    vertexColorsBuffer_ = device_->newBuffer(colorDataSize, MTL::StorageModeShared);
    transformBuffer_ = device_->newBuffer(transformDataSize, MTL::StorageModeShared);

    Uniforms uniforms;
    uniforms.projection = projectionMatrix_;
    uniforms.view = view;
    uniformsBuffer_ = device_->newBuffer(sizeof(Uniforms), MTL::StorageModeShared);

    memcpy(indexBuffer_->contents(), indices, indicesDataSize);
    memcpy(vertexPositionsBuffer_->contents(), positions, positionsDataSize);
    memcpy(vertexUVsBuffer_->contents(), uvs, uvsDataSize);
    memcpy(vertexColorsBuffer_->contents(), colors, colorDataSize);
    memcpy(transformBuffer_->contents(), &transform, transformDataSize);
    memcpy(uniformsBuffer_->contents(), &uniforms, sizeof(Uniforms));

    indexBuffer_->didModifyRange(NS::Range::Make(0, indexBuffer_->length()));
    vertexPositionsBuffer_->didModifyRange(NS::Range::Make(0, vertexPositionsBuffer_->length()));
    vertexUVsBuffer_->didModifyRange(NS::Range::Make(0, vertexUVsBuffer_->length()));
    vertexColorsBuffer_->didModifyRange(NS::Range::Make(0, vertexColorsBuffer_->length()));
    transformBuffer_->didModifyRange(NS::Range::Make(0, transformBuffer_->length()));
    uniformsBuffer_->didModifyRange(NS::Range::Make(0, uniformsBuffer_->length()));
    pPool->release();
}
}   // namespace snowpulse
