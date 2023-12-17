#include "application_opengl.h"
#include "../application.h"

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "../timer.h"
#include "../input.h"
#include "../windows/input_windows.h"

namespace snowpulse {
ApplicationOpenGL::ApplicationOpenGL() : Application() {
    platform_ = Platform::kOpenGL;
    platformString_ = "OpenGL";
}

ApplicationOpenGL::~ApplicationOpenGL() {
#ifdef SPDEBUG
    std::cout << "ApplicationOpenGL destroyed." << std::endl;
#endif
}

void kWindowResizeCallback(GLFWwindow* window, int width, int height) {
    Application::GetInstance()->SetScreenSize(Vector2Int(width, height));
}

bool ApplicationOpenGL::Initialize(const Vector2Int& resolutionSize, const Vector2Int& screenSize) {
    if (!snowpulse::Application::Initialize(resolutionSize, screenSize)) {
        return false;
    }

#ifdef SPDEBUG
    std::cout << "Starting GLFW context, OpenGL 4.1" << std::endl;
#endif

    if (!glfwInit()) {
#ifdef SPDEBUG
        std::cerr << "Failed to initialize GLFW" << std::endl;
#endif
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    
    auto isFullscreen = false;
    if (isFullscreen) {
        auto primaryMonitor = glfwGetPrimaryMonitor();
        const auto mode = glfwGetVideoMode(primaryMonitor);
        auto fullscreenSize = Vector2Int(mode->width, mode->height);
        Application::SetScreenSize(fullscreenSize);
        window_ = glfwCreateWindow(mode->width, mode->height, "Snowpulse", primaryMonitor, NULL);
    }
    else {
        window_ = glfwCreateWindow(screenSize_.x, screenSize_.y, "Snowpulse", NULL, NULL);
    }

    if (!window_) {
#ifdef SPDEBUG
        std::cerr << "Failed to create GLFW window" << std::endl;
#endif
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window_);
    glfwSetWindowSizeCallback(window_, kWindowResizeCallback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
#ifdef SPDEBUG
        std::cerr << "Failed to initialize GLAD" << std::endl;
#endif
        return false;
    }

    graphics_ = GraphicsOpenGL::Create();
    if (!graphics_ || !graphics_->Initialize(resolutionSize_, screenSize_)) {
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
    ImGui_ImplOpenGL3_Init("#version 130");
    //ImGui_ImplGlfw_InitForOpenGL(window_, true);

#ifdef SPDEBUG
    std::cout << "ApplicationOpenGL initialized." << std::endl;
#endif
    return true;
}

void ApplicationOpenGL::SetScreenSize(const Vector2Int& screenSize) {
    Application::SetScreenSize(screenSize);
    if (graphics_) {
        glViewport(0, 0, screenSize_.x, screenSize_.y);
        graphics_->UpdateProjectionMatrix(resolutionSize_);
    }
    if (ImGui::GetCurrentContext()) {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(screenSize_.x, screenSize_.y);
    }
}

void ApplicationOpenGL::Shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    //ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (game_) {
        game_->Shutdown();
    }
    if (graphics_) {
        graphics_->Shutdown();
    }
#ifdef SPDEBUG
    std::cout << "ApplicationOpenGL shutdown." << std::endl;
#endif
}

void ApplicationOpenGL::SetScreenScaleFactor(float scaleFactor) {
    Application::SetScreenScaleFactor(scaleFactor);
    ImGuiIO& io = ImGui::GetIO();
    io.DisplayFramebufferScale = ImVec2(scaleFactor, scaleFactor);
}

void kKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto input = static_cast<InputWindows*>(Input::GetInstance());
    if (action == GLFW_PRESS) {
        input->ProcessInputs(key, true);
    }
    else if (action == GLFW_RELEASE) {
        input->ProcessInputs(key, false);
    }
}

void kTextCallback(GLFWwindow* window, unsigned int codepoint) {
    auto input = static_cast<InputWindows*>(Input::GetInstance());
    input->ProcessInputs(input->CodepointToString(codepoint));
}

void kMouseCallback(GLFWwindow* window, int button, int action, int mods) {
    auto input = static_cast<InputWindows*>(Input::GetInstance());
    auto application = Application::GetInstance();
    auto resolutionSize = application->GetResolutionSize();
    auto screenSize = application->GetScreenSize();
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    if (action == GLFW_PRESS) {
        input->ProcessInputs(resolutionSize, screenSize, Vector2((float)x, (float)screenSize.y - (float)y), button, true);
    }
    else if (action == GLFW_RELEASE) {
        input->ProcessInputs(resolutionSize, screenSize, Vector2((float)x, (float)screenSize.y - (float)y), button, false);
    }
}

void kMouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    auto input = static_cast<InputWindows*>(Input::GetInstance());
    input->ProcessInputs(Vector2(xOffset, yOffset));
}

void ApplicationOpenGL::Run() {
    Timer timer;
    timer.Start();

    Timer frameTimer;
    frameTimer.Start();

    game_->Start();
    actionManager_->Start();

    glfwSetKeyCallback(window_, kKeyCallback);
    glfwSetCharCallback(window_, kTextCallback);
    glfwSetMouseButtonCallback(window_, kMouseCallback);
    glfwSetScrollCallback(window_, kMouseScrollCallback);

    while (!glfwWindowShouldClose(window_)) {

        glfwPollEvents();
        auto input = static_cast<InputWindows*>(Input::GetInstance());
        input->ProcessInputs(resolutionSize_, screenSize_, window_);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        if (graphics_->IsDepthTesting()) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        else {
            glClear(GL_COLOR_BUFFER_BIT);
        }

        auto deltaTime = GetDeltaTime(frameTimer_);
        nodeStarter_.StartNodes();
        Application::Update(deltaTime);
        actionManager_->Update(deltaTime);
        game_->UpdateScene(deltaTime);

        ImGui_ImplOpenGL3_NewFrame();
        //ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        game_->DrawScene(graphics_.get());

        auto batches = graphics_->GetRenderQueue()->PopAllData();
        for (int i = 0; i < 1; i++) {
            for (auto b : batches) {
                switch (b->blendMode) {
                case BlendMode::kDisabled:
                    glDisable(GL_BLEND);
                    break;
                case BlendMode::kNormal:
                    glEnable(GL_BLEND);
                    b->isPremultiplied ? glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA) : glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    break;
                case BlendMode::kAdditive:
                    glEnable(GL_BLEND);
                    b->isPremultiplied ? glBlendFunc(GL_ONE, GL_ONE) : glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                    break;
                case BlendMode::kMultiply:
                    glEnable(GL_BLEND);
                    b->isPremultiplied ? glBlendFunc(GL_DST_COLOR, GL_ZERO) : glBlendFunc(GL_DST_COLOR, GL_ZERO);
                    break;
                case BlendMode::kScreen:
                    glEnable(GL_BLEND);
                    b->isPremultiplied ? glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR) : glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);
                    break;
                }

                GLuint VBO, VAO, EBO, matrixVBO;
                float vertices[b->vertices.size()];
                unsigned short indices[b->indices.size()];
                int i = 0;
                for (const auto ver : b->vertices) {
                    vertices[i++] = ver;
                }
                i = 0;
                for (const auto ind : b->indices) {
                    indices[i++] = ind;
                }

                glGenVertexArrays(1, &VAO);
                glGenBuffers(1, &VBO);
                glGenBuffers(1, &EBO);
                glGenBuffers(1, &matrixVBO);
                glBindVertexArray(VAO);

                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

                glVertexAttribPointer(b->vertexShaderLocation, b->vertexSectionCount, GL_FLOAT, GL_FALSE, b->vertexPlusUVPlusColorCount * sizeof(float), (void*)0);
                glEnableVertexAttribArray(b->vertexShaderLocation);

                glVertexAttribPointer(b->uVShaderLocation, b->uVSectionCount, GL_FLOAT, GL_FALSE, b->vertexPlusUVPlusColorCount * sizeof(float), (void*)(b->vertexSectionCount * sizeof(float)));
                glEnableVertexAttribArray(b->uVShaderLocation);

                glVertexAttribPointer(b->colorShaderLocation, b->colorSectionCount, GL_FLOAT, GL_FALSE, b->vertexPlusUVPlusColorCount * sizeof(float), (void*)((b->vertexSectionCount + b->uVSectionCount) * sizeof(float)));
                glEnableVertexAttribArray(b->colorShaderLocation);

                glBindBuffer(GL_ARRAY_BUFFER, matrixVBO);
                glBufferData(GL_ARRAY_BUFFER, b->vertexCount * sizeof(glm::mat4), b->matrices.data(), GL_STATIC_DRAW);

                for (int i = 0; i < 4; i++) {
                    glVertexAttribPointer(b->transformMatrixShaderLocation + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
                    glEnableVertexAttribArray(b->transformMatrixShaderLocation + i);
                }

                glUseProgram(b->shaderProgram);
                glUniformMatrix4fv(b->viewMatrixShaderLocation, 1, GL_FALSE, &(b->viewMatrix[0].x));
                glUniformMatrix4fv(b->projectionMatrixShaderLocation, 1, GL_FALSE, &(b->projectionMatrix[0].x));
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, b->texture);
                glBindVertexArray(VAO);

                switch (b->textureFiltering) {
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

                glDrawElements(b->drawMode, b->indexCount, GL_UNSIGNED_SHORT, 0);
                glBindVertexArray(0);

                glDeleteBuffers(1, &VBO);
                glDeleteBuffers(1, &EBO);
                glDeleteBuffers(1, &matrixVBO);
                glDeleteVertexArrays(1, &VAO);

#ifdef SPDEBUG
                auto error = glGetError();
                if (error != GL_NO_ERROR) {
                    std::cout << "GL Error: " << error << std::endl;
                }
#endif
            }
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window_);

        graphics_->ClearRenderBatchGroups();
        Input::GetInstance()->ClearLastFrameData();
#ifdef SPDEBUG
        std::cout << "Rendered " << batches.size() << " batch/es." << std::endl;
#endif
    }

    glfwTerminate();

    auto elapsed = timer.GetElapsedInSeconds();
#ifdef SPDEBUG
    std::cout << "Application ran for " << elapsed << " seconds." << std::endl;
#endif
}

void ApplicationOpenGL::RunFrame() {
/*    auto device = graphics_->GetDevice();
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
    for (auto b : batches) {
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
            assert(false);
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
#endif*/
}
}   // namespace snowpulse
