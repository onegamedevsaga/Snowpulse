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

#include <imgui.h>
#include <imgui_impl_metal.h>

#include "../timer.h"
#include "../input.h"
#include "view_delegate_metal.h"

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
    if (!Application::Initialize(resolutionSize, screenSize)) {
        return false;
    }

    viewDelegate_ = std::make_shared<ViewDelegateMetal>(this);
    view_ = static_cast<MTK::View*>(view);
    view_->setDelegate(viewDelegate_.get());

    graphics_ = GraphicsMetal::Create();
    if (!graphics_ || !graphics_->Initialize(resolutionSize_, screenSize_, view)) {
#ifdef SPDEBUG
        std::cout << "ERROR: Application wasn't able to initialize graphics on this platform." << std::endl;
#endif
        return false;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    io.DeltaTime = 1.0f / 60.0f;
    io.DisplaySize = ImVec2(screenSize_.x, screenSize_.y);
    ImGui_ImplMetal_Init(graphics_->GetDevice());
    return true;
}

void ApplicationMetal::SetScreenSize(const Vector2Int& screenSize) {
    Application::SetScreenSize(screenSize);
    if (graphics_) {
        graphics_->UpdateProjectionMatrix(resolutionSize_);
    }
    if (ImGui::GetCurrentContext()) {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(screenSize_.x, screenSize_.y);
    }
}

void ApplicationMetal::Shutdown() {
    ImGui_ImplMetal_Shutdown();
    ImGui::DestroyContext();

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

void ApplicationMetal::SetScreenScaleFactor(float scaleFactor) {
    Application::SetScreenScaleFactor(scaleFactor);
    ImGuiIO& io = ImGui::GetIO();
    io.DisplayFramebufferScale = ImVec2(scaleFactor, scaleFactor);
}

void ApplicationMetal::RunFrame() {
    auto device = graphics_->GetDevice();
    auto commandQueue = graphics_->GetCommandQueue();

    //glfwPollEvents();
#if SNOWPULSE_PLATFORM_MACOS
    auto input = static_cast<InputMacOS*>(Input::GetInstance());
#elif SNOWPULSE_PLATFORM_IOS
    auto input = static_cast<InputIOS*>(Input::GetInstance());
#endif

    auto deltaTime = GetDeltaTime(frameTimer_);
    nodeStarter_.StartNodes();
    Application::Update(deltaTime);
    actionManager_->Update(deltaTime);
    game_->UpdateScene(deltaTime);

    auto commandBuffer = commandQueue->commandBuffer();
    auto renderPassDesc = view_->currentRenderPassDescriptor();
    auto commandEncoder = commandBuffer->renderCommandEncoder(renderPassDesc);

    ImGui_ImplMetal_NewFrame(renderPassDesc);
    ImGui::NewFrame();

    game_->DrawScene(graphics_.get());

    auto batches = graphics_->GetRenderQueue()->PopAllData();
    for(auto b : batches) {
        NS::Error* error = SPNULL;
        auto library = device->newLibrary(NS::String::string(b->shaderProgram.c_str(), NS::StringEncoding::UTF8StringEncoding), SPNULL, &error);
        if (!library) {
#ifdef SPDEBUG
            __builtin_printf("%s", error->localizedDescription()->utf8String());
            assert(false);
#endif
        }

        auto vertexFn = library->newFunction(NS::String::string("vertexMain", NS::StringEncoding::UTF8StringEncoding));
        auto fragFn = library->newFunction(NS::String::string("fragmentMain", NS::StringEncoding::UTF8StringEncoding));
        auto renderPipelineDesc = MTL::RenderPipelineDescriptor::alloc()->init();
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
        const size_t transformDataSize = b->vertexCount * sizeof(simd::float4x4);

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

        simd::float4x4 transformMatrices[b->vertexCount];
        for (int i = 0, n = (int)b->transformMatrices.size(); i < n; i++) {
            transformMatrices[i] = b->transformMatrices[i];
        }

        memcpy(indexBuffer->contents(), indices, indicesDataSize);
        memcpy(vertexPositionsBuffer->contents(), positions, positionsDataSize);
        memcpy(vertexUVsBuffer->contents(), uvs, uvsDataSize);
        memcpy(vertexColorsBuffer->contents(), colors, colorDataSize);
        memcpy(transformBuffer->contents(), transformMatrices, transformDataSize);
        memcpy(uniformsBuffer->contents(), &uniforms, sizeof(Uniforms));

        auto samplerDesc = MTL::SamplerDescriptor::alloc()->init();
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

        auto samplerState = device->newSamplerState(samplerDesc);
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

    ImGui::Render();
    ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), commandBuffer, commandEncoder);

    commandEncoder->endEncoding();
    commandBuffer->presentDrawable(view_->currentDrawable());
    commandBuffer->commit();

    graphics_->ClearRenderBatchGroups();
    input->ClearLastFrameData();
#ifdef SPDEBUG
     std::cout << "Rendered " << batches.size() << " batch/es." << std::endl;
#endif
}
}   // namespace snowpulse
