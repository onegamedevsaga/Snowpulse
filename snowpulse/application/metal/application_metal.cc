#include "application_metal.h"

#include <iostream>
#include <string>

#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#include <Metal/Metal.hpp>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>
#include <simd/simd.h>
#undef NS_PRIVATE_IMPLEMENTATION
#undef MTL_PRIVATE_IMPLEMENTATION
#undef MTK_PRIVATE_IMPLEMENTATION
#undef CA_PRIVATE_IMPLEMENTATION

#include "../timer.h"
#include "../input.h"
#include "view_delegate_metal.h"

#if SNOWPULSE_PLATFORM_MACOS
#include "../macos/application_macos.h"
#elif SNOWPULSE_PLATFORM_IOS
#include "../ios/application_ios.h"
#endif

namespace snowpulse {

struct Uniforms {
    simd::float4x4 projection;
    simd::float4x4 view;
};

ApplicationMetal::ApplicationMetal() : Application() {
    platform_ = Platform::kMetal;
    platformString_ = "Metal";
}

ApplicationMetal::~ApplicationMetal() {
#ifdef SPDEBUG
    std::cout << "ApplicationMetal destroyed." << std::endl;
#endif
}

bool ApplicationMetal::Initialize(const Vector2Int& resolutionSize, const Vector2Int& screenSize, void* view) {
    if (!Application::Initialize(resolutionSize)) {
        return false;
    }
    
    graphics_ = GraphicsMetal::Create();
    if (!graphics_ || !graphics_->Initialize(resolutionSize, screenSize, view)) {
#ifdef SPDEBUG
        std::cout << "ERROR: Application wasn't able to initialize graphics on this platform." << std::endl;
#endif
        return false;
    }
    
    viewDelegate_ = std::make_shared<ViewDelegateMetal>(this);
    view_ = static_cast<MTK::View*>(view);
    view_->setDelegate(viewDelegate_.get());
    return true;
}

void ApplicationMetal::Shutdown() {
    if (game_) {
        game_->Shutdown();
    }
    if (graphics_) {
        graphics_->Shutdown();
    }
#ifdef SPDEBUG
    std::cout << "ApplicationMetal shutdown." << std::endl;
#endif
}

void ApplicationMetal::RunFrame() {
    using NS::StringEncoding::UTF8StringEncoding;

    auto device = graphics_->GetDevice();
    auto commandQueue = graphics_->GetCommandQueue();

    //glfwPollEvents();
#if SNOWPULSE_PLATFORM_MACOS
    auto input = static_cast<InputMacOS*>(Input::GetInstance());
    input->ProcessInputs(resolutionSize_, screenSize_, window_);
#elif SNOWPULSE_PLATFORM_IOS
    auto input = static_cast<InputIOS*>(Input::GetInstance());
    input->ProcessInputs();
#endif

    auto deltaTime = GetDeltaTime(frameTimer_);
    nodeStarter_.StartNodes();
    actionManager_->Update(deltaTime);
    game_->UpdateScene(deltaTime);
    game_->DrawScene(graphics_.get());

    MTL::CommandBuffer* commandBuffer = commandQueue->commandBuffer();
    MTL::RenderPassDescriptor* renderPassDesc = view_->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder* commandEncoder = commandBuffer->renderCommandEncoder(renderPassDesc);

    auto batches = graphics_->GetRenderQueue()->PopAllData();
    for(auto b : batches) {
        NS::Error* error = SPNULL;
        MTL::Library* library = device->newLibrary(NS::String::string(b->shaderProgram.c_str(), UTF8StringEncoding), SPNULL, &error);
        if (!library) {
#ifdef SPDEBUG
            __builtin_printf("%s", error->localizedDescription()->utf8String());
            assert(false);
#endif
        }

        MTL::Function* vertexFn = library->newFunction(NS::String::string("vertexMain", UTF8StringEncoding));
        MTL::Function* fragFn = library->newFunction(NS::String::string("fragmentMain", UTF8StringEncoding));
        MTL::RenderPipelineDescriptor* renderPipelineDesc = MTL::RenderPipelineDescriptor::alloc()->init();
        renderPipelineDesc->setVertexFunction(vertexFn);
        renderPipelineDesc->setFragmentFunction(fragFn);

        auto colorDesc = renderPipelineDesc->colorAttachments()->object(0);
        colorDesc->setPixelFormat(view_->colorPixelFormat());
        colorDesc->setBlendingEnabled(b->blendMode != BlendMode::kDisabled);
        colorDesc->setRgbBlendOperation(MTL::BlendOperationAdd);
        colorDesc->setAlphaBlendOperation(MTL::BlendOperationAdd);

        switch (b->blendMode) {
            case BlendMode::kDisabled:
                break;
            case BlendMode::kNormal:
                colorDesc->setSourceRGBBlendFactor(b->isPremultiplied ? MTL::BlendFactorOne : MTL::BlendFactorSourceAlpha);
                colorDesc->setSourceAlphaBlendFactor(b->isPremultiplied ? MTL::BlendFactorOne : MTL::BlendFactorSourceAlpha);
                colorDesc->setDestinationRGBBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
                colorDesc->setDestinationAlphaBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
                break;
            case BlendMode::kAdditive:
                colorDesc->setSourceRGBBlendFactor(b->isPremultiplied ? MTL::BlendFactorOne : MTL::BlendFactorSourceAlpha);
                colorDesc->setSourceAlphaBlendFactor(b->isPremultiplied ? MTL::BlendFactorOne : MTL::BlendFactorSourceAlpha);
                colorDesc->setDestinationRGBBlendFactor(MTL::BlendFactorOne);
                colorDesc->setDestinationAlphaBlendFactor(MTL::BlendFactorOne);
                break;
            case BlendMode::kMultiply:
                colorDesc->setSourceRGBBlendFactor(b->isPremultiplied ? MTL::BlendFactorDestinationColor : MTL::BlendFactorDestinationColor);
                colorDesc->setSourceAlphaBlendFactor(b->isPremultiplied ? MTL::BlendFactorDestinationColor : MTL::BlendFactorDestinationColor);
                colorDesc->setDestinationRGBBlendFactor(MTL::BlendFactorZero);
                colorDesc->setDestinationAlphaBlendFactor(MTL::BlendFactorZero);
                break;
            case BlendMode::kScreen:
                colorDesc->setSourceRGBBlendFactor(b->isPremultiplied ? MTL::BlendFactorOne : MTL::BlendFactorSourceAlpha);
                colorDesc->setSourceAlphaBlendFactor(b->isPremultiplied ? MTL::BlendFactorOne : MTL::BlendFactorSourceAlpha);
                colorDesc->setDestinationRGBBlendFactor(MTL::BlendFactorOneMinusSourceColor);
                colorDesc->setDestinationAlphaBlendFactor(MTL::BlendFactorOneMinusSourceColor);
                break;
        }

        auto renderPipelineState = device->newRenderPipelineState(renderPipelineDesc, &error);
        if (!renderPipelineState) {
#ifdef SPDEBUG
            __builtin_printf("%s", error->localizedDescription()->utf8String());
            assert( false );
#endif
        }

        
        
        
        
        
        const size_t indicesDataSize = b->indexCount * sizeof(uint16_t);
        const size_t positionsDataSize = b->vertexCount * sizeof(simd::float3);
        const size_t uvsDataSize = b->vertexCount * sizeof(simd::float2);
        const size_t colorDataSize = b->vertexCount * sizeof(simd::float4);
        const size_t transformDataSize = sizeof(simd::float4x4);

        auto indexBuffer = device->newBuffer(indicesDataSize, MTL::StorageModeShared);
        auto vertexPositionsBuffer = device->newBuffer(positionsDataSize, MTL::StorageModeShared);
        auto vertexUVsBuffer = device->newBuffer(uvsDataSize, MTL::StorageModeShared);
        auto vertexColorsBuffer = device->newBuffer(colorDataSize, MTL::StorageModeShared);
        auto transformBuffer = device->newBuffer(transformDataSize, MTL::StorageModeShared);

        Uniforms uniforms;
        uniforms.projection = b->projectionMatrix;
        uniforms.view = b->viewMatrix;
        auto uniformsBuffer = device->newBuffer(sizeof(Uniforms), MTL::StorageModeShared);

        uint16_t indices[b->indexCount];
        for (int i = 0, n = (int)b->indices.size(); i < n; i++) {
            indices[i] = b->indices[i];
        }

        simd::float3 positions[b->vertexCount];
        for (int i = 0, n = (int)b->vertices.size(); i < n; i++) {
            positions[i] = b->vertices[i];
        }

        simd::float2 uvs[b->vertexCount];
        for (int i = 0, n = (int)b->uvs.size(); i < n; i++) {
            uvs[i] = b->uvs[i];
        }

        simd::float4 colors[b->vertexCount];
        for (int i = 0, n = (int)b->colors.size(); i < n; i++) {
            colors[i] = b->colors[i];
        }

        memcpy(indexBuffer->contents(), indices, indicesDataSize);
        memcpy(vertexPositionsBuffer->contents(), positions, positionsDataSize);
        memcpy(vertexUVsBuffer->contents(), uvs, uvsDataSize);
        memcpy(vertexColorsBuffer->contents(), colors, colorDataSize);
        memcpy(transformBuffer->contents(), &b->transformMatrix, transformDataSize);
        memcpy(uniformsBuffer->contents(), &uniforms, sizeof(Uniforms));

        indexBuffer->didModifyRange(NS::Range::Make(0, indexBuffer->length()));
        vertexPositionsBuffer->didModifyRange(NS::Range::Make(0, vertexPositionsBuffer->length()));
        vertexUVsBuffer->didModifyRange(NS::Range::Make(0, vertexUVsBuffer->length()));
        vertexColorsBuffer->didModifyRange(NS::Range::Make(0, vertexColorsBuffer->length()));
        transformBuffer->didModifyRange(NS::Range::Make(0, transformBuffer->length()));
        uniformsBuffer->didModifyRange(NS::Range::Make(0, uniformsBuffer->length()));

        MTL::SamplerDescriptor* samplerDesc = MTL::SamplerDescriptor::alloc()->init();
        samplerDesc->setMinFilter(MTL::SamplerMinMagFilterLinear);
        samplerDesc->setMagFilter(MTL::SamplerMinMagFilterLinear);
        samplerDesc->setSAddressMode(MTL::SamplerAddressModeRepeat);
        samplerDesc->setTAddressMode(MTL::SamplerAddressModeRepeat);
        switch (b->textureFiltering) {
            case TextureFiltering::kPoint:
                samplerDesc->setMinFilter(MTL::SamplerMinMagFilterNearest);
                samplerDesc->setMagFilter(MTL::SamplerMinMagFilterNearest);
                break;
            case TextureFiltering::kBilinear:
                samplerDesc->setMinFilter(MTL::SamplerMinMagFilterLinear);
                samplerDesc->setMagFilter(MTL::SamplerMinMagFilterLinear);
                break;
            case TextureFiltering::kTrilinear:
                samplerDesc->setMinFilter(MTL::SamplerMinMagFilterLinear);
                samplerDesc->setMagFilter(MTL::SamplerMinMagFilterLinear);
                samplerDesc->setMipFilter(MTL::SamplerMipFilterLinear);
                break;
            case TextureFiltering::kAnisotropic:
                samplerDesc->setMinFilter(MTL::SamplerMinMagFilterLinear);
                samplerDesc->setMagFilter(MTL::SamplerMinMagFilterLinear);
                samplerDesc->setMaxAnisotropy(8);
                break;
        }

        
        MTL::SamplerState* samplerState = device->newSamplerState(samplerDesc);
        commandEncoder->setRenderPipelineState(renderPipelineState);
        commandEncoder->setVertexBuffer(vertexPositionsBuffer, 0, b->positionBufferIndex);
        commandEncoder->setVertexBuffer(vertexUVsBuffer, 0, b->uVBufferIndex);
        commandEncoder->setVertexBuffer(vertexColorsBuffer, 0, b->colorBufferIndex);
        commandEncoder->setVertexBuffer(transformBuffer, 0, b->transformBufferIndex);
        commandEncoder->setVertexBuffer(uniformsBuffer, 0, b->uniformsBufferIndex);
        commandEncoder->setFragmentTexture(b->texture, 0);
        commandEncoder->setFragmentSamplerState(samplerState, 0);
        commandEncoder->drawIndexedPrimitives((MTL::PrimitiveType)b->drawMode, NS::UInteger(b->indexCount), MTL::IndexTypeUInt16, indexBuffer, NS::UInteger(0), NS::UInteger(1));


        indexBuffer->release();
        vertexPositionsBuffer->release();
        vertexUVsBuffer->release();
        vertexColorsBuffer->release();
        transformBuffer->release();
        uniformsBuffer->release();

        samplerDesc->release();
        samplerState->release();
        vertexFn->release();
        fragFn->release();
        renderPipelineDesc->release();
        library->release();
        renderPipelineState->release();
    }
    
    commandEncoder->endEncoding();
    commandBuffer->presentDrawable(view_->currentDrawable());
    commandBuffer->commit();

    graphics_->ClearRenderBatchGroups();
#ifdef SPDEBUG
     std::cout << "Rendered " << batches.size() << " batch/es." << std::endl;
#endif
}
}   // namespace snowpulse
