#include "graphics_metal.h"

#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION

#include <glm/gtc/type_ptr.hpp>
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

const char* kShaderSrc = R"(
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
        o.position = uniforms.projection * uniforms.view * transform[vertexId] * float4(positions[vertexId], 1.0);
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

std::shared_ptr<GraphicsMetal> GraphicsMetal::Create() {
    auto graphics = new GraphicsMetal();
    return std::shared_ptr<GraphicsMetal>(graphics);
}

GraphicsMetal::GraphicsMetal() {
}

bool GraphicsMetal::Initialize(const Vector2Int& resolution, const Vector2Int& screenSize, void* view) {
    camera_ = Camera::Create(resolution);
    renderQueue_ = RenderQueueMetal::Create();
    view_ = static_cast<MTK::View*>(view);
    view_->setColorPixelFormat(MTL::PixelFormatBGRA8Unorm_sRGB);
    view_->setClearColor(MTL::ClearColor::Make(0.2f, 0.3f, 0.3f, 1.0));
    device_ = view_->device();
    commandQueue_ = device_->newCommandQueue();
    renderQueue_ = RenderQueueMetal::Create();

    UpdateProjectionMatrix(resolution);
    LoadTexture(kSpriteDefault, PathType::kDefaults);

#ifdef SPDEBUG
    std::cout << "GraphicsMetal initialized." << std::endl;
#endif

    return true;
}

void GraphicsMetal::UpdateProjectionMatrix(const Vector2Int& resolution) {
    auto ortho = glm::ortho(0.0f, (float)resolution.x, 0.0f, (float)resolution.y, -100.0f, 100.0f);
    memcpy(projectionMatrix_.columns, glm::value_ptr(ortho), sizeof(projectionMatrix_.columns));
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
    memcpy(glm::value_ptr(transform), matrix.data, sizeof(matrix.data));
    transform = glm::inverse(transform);
    memcpy(matrix.data, glm::value_ptr(transform), sizeof(matrix.data));
    return matrix;
}

void GraphicsMetal::LoadTexture(std::string filename, PathType pathType) {
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

void GraphicsMetal::LoadTexture(std::string name, unsigned char* bitmap, Vector2Int size) {
    auto width = size.x;
    auto height = size.y;
    NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();
    MTL::TextureDescriptor* texDesc = MTL::TextureDescriptor::alloc()->init();
    texDesc->setWidth(width);
    texDesc->setHeight(height);
    texDesc->setPixelFormat(MTL::PixelFormatRGBA8Unorm_sRGB);
    texDesc->setTextureType(MTL::TextureType2D);
    texDesc->setStorageMode(MTL::StorageModeShared);
    texDesc->setUsage(MTL::TextureUsageShaderRead);

    MTL::Texture* texture = device_->newTexture(texDesc);
    MTL::Region region = MTL::Region(0, 0, width, height);
    texture->replaceRegion(region, 0, bitmap, width * 4);

    textures_[name] = texture;
    textureSizes_[texture] = Vector2(width, height);
    texDesc->release();
    pPool->release();
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

void GraphicsMetal::DrawMesh(Vertex* vertices, unsigned int vertexCount, unsigned short* indices, unsigned int indexCount, std::string textureFilename, int sortOrder, BlendMode blendMode, TextureFiltering filtering, bool isPremultiplied, Matrix4x4 transformMatrix, int batchGroup) {
    auto batch = std::make_shared<RenderBatchDataMetal>();
    batch->drawMode = MTL::PrimitiveType::PrimitiveTypeTriangle;
    batch->shaderProgram = kShaderSrc;
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

    simd::float4x4 transMat;
    memcpy(transMat.columns, transformMatrix.data, sizeof(transMat.columns));

    batch->vertices.reserve(batch->vertexCount);
    batch->uvs.reserve(batch->vertexCount);
    batch->colors.reserve(batch->vertexCount);
    batch->transformMatrices.reserve(batch->vertexCount);
    batch->indices.reserve(batch->indexCount);

    for (int i = 0; i < batch->vertexCount; i++) {
        auto pos = vertices[i].position;
        auto uv = vertices[i].uv;
        auto color = vertices[i].color;
        simd::float3 v = { pos.x, pos.y, pos.z };
        simd::float2 u { uv.x, uv.y };
        simd::float4 c { color.r, color.g, color.b, color.a };

        batch->vertices.push_back(v);
        batch->uvs.push_back(u);
        batch->colors.push_back(c);
        batch->transformMatrices.push_back(transMat);
    }

    for (int i = 0; i < batch->indexCount; i++) {
        batch->indices.push_back(indices[i]);
    }

    auto cameraView = camera_->GetMatrix();
    memcpy(batch->viewMatrix.columns, cameraView.data, sizeof(batch->viewMatrix.columns));

    batch->projectionMatrix = projectionMatrix_;
    batch->positionBufferIndex = 0;
    batch->uVBufferIndex = 1;
    batch->colorBufferIndex = 2;
    batch->transformBufferIndex = 3;
    batch->uniformsBufferIndex = 4;

    if (batchGroup >= 0 && batchGroup < (int)renderBatchGroups_.size()) {
        auto group = renderBatchGroups_[batchGroup];
        group->AddBatch(batch);
    }
    else {
        renderQueue_->Push(batch);
    }
}

void GraphicsMetal::DrawSprite(Vector2 size, std::string textureFilename, Matrix4x4 transformMatrix, Color color, int sortOrder, BlendMode blendMode, TextureFiltering filtering, bool isPremultiplied, Vector2 uvLowerLeft, Vector2 uvUpperRight, int batchGroup) {
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
