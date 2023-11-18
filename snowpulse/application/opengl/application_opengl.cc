#include "application_opengl.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gtc/type_ptr.hpp>

#include "../timer.h"

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

bool ApplicationOpenGL::Initialize() {
    if (Application::Initialize()) {
        graphics_ = GraphicsOpenGL::Create();
        if (!graphics_) {
#ifdef SPDEBUG
            std::cout << "ERROR: Application wasn't able to initialize graphics on this platform." << std::endl;
#endif
            return false;
        }

        renderQueue_ = RenderQueueOpenGL::Create();
        graphics_->Initialize(Vector2Int(1920, 1080), camera_.get(), renderQueue_.get(), Vector2Int(1067, 600));
        input_ = InputOpenGL::Create();

#ifdef SPDEBUG
        std::cout << "ApplicationOpenGL initialized." << std::endl;
#endif
        return true;
    }

    return false;
}

void ApplicationOpenGL::Run() {
    Timer timer;
    timer.Start();

    Timer frameTimer;
    frameTimer.Start();

    game_->Start();
    actionManager_->Start();

    while (!glfwWindowShouldClose(graphics_->GetWindow())) {

        glfwPollEvents();
        input_->ProcessInputs(graphics_.get());

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        if (graphics_->IsDepthTesting()) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        else {
            glClear(GL_COLOR_BUFFER_BIT);
        }

        auto deltaTime = GetDeltaTime(frameTimer);
        nodeStarter_.StartNodes();
        actionManager_->Update(deltaTime);
        game_->UpdateScene(deltaTime);
        game_->DrawScene(graphics_.get());

        auto batches = renderQueue_->PopAll();
        for (int i = 0; i < 1; i++) {
            for(auto b : batches) {

                switch (b.blendMode) {
                    case BlendMode::kDisabled:
                        glDisable(GL_BLEND);
                        break;
                    case BlendMode::kNormal:
                        glEnable(GL_BLEND);
                        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                        break;
                    case BlendMode::kAdditive:
                        glEnable(GL_BLEND);
                        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                        break;
                    case BlendMode::kMultiply:
                        glEnable(GL_BLEND);
                        glBlendFunc(GL_DST_COLOR, GL_ZERO);
                        break;
                    case BlendMode::kScreen:
                        glEnable(GL_BLEND);
                        glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
                        break;
                }

                GLuint VBO, VAO, EBO, matrixVBO;
                float vertices[b.vertices.size()];
                int indices[b.indices.size()];
                int i = 0;
                for (const auto ver : b.vertices) {
                    vertices[i++] = ver;
                }
                i = 0;
                for (const auto ind : b.indices) {
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

                glVertexAttribPointer(b.vertexShaderLocation, b.vertexSectionCount, GL_FLOAT, GL_FALSE, b.vertexPlusUVPlusColorCount * sizeof(float), (void*)0);
                glEnableVertexAttribArray(b.vertexShaderLocation);

                glVertexAttribPointer(b.uVShaderLocation, b.uVSectionCount, GL_FLOAT, GL_FALSE, b.vertexPlusUVPlusColorCount * sizeof(float), (void*)(b.vertexSectionCount * sizeof(float)));
                glEnableVertexAttribArray(b.uVShaderLocation);

                glVertexAttribPointer(b.colorShaderLocation, b.colorSectionCount, GL_FLOAT, GL_FALSE, b.vertexPlusUVPlusColorCount * sizeof(float), (void*)((b.vertexSectionCount + b.uVSectionCount) * sizeof(float)));
                glEnableVertexAttribArray(b.colorShaderLocation);

                glBindBuffer(GL_ARRAY_BUFFER, matrixVBO);
                glBufferData(GL_ARRAY_BUFFER, b.vertexCount * sizeof(glm::mat4), b.matrices.data(), GL_STATIC_DRAW);

                for (int i = 0; i < 4; i++) {
                    glVertexAttribPointer(b.transformMatrixShaderLocation + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
                    glEnableVertexAttribArray(b.transformMatrixShaderLocation + i);
                }

                glUseProgram(b.shaderProgram);
                glUniformMatrix4fv(b.viewMatrixShaderLocation, 1, GL_FALSE, &(b.viewMatrix[0].x));
                glUniformMatrix4fv(b.projectionMatrixShaderLocation, 1, GL_FALSE, &(b.projectionMatrix[0].x));
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, b.texture);
                glBindVertexArray(VAO);
                glDrawElements(b.drawMode, 18, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);

#ifdef SPDEBUG
                auto error = glGetError();
                if (error != GL_NO_ERROR) {
                    std::cout << "GL Error: " << error << std::endl;
                }
#endif
            }
        }

#ifdef SPDEBUG
    std::cout << "Rendered " << batches.size() << " batch/es." << std::endl;
#endif

        glfwSwapBuffers(graphics_->GetWindow());
    }

    glfwTerminate();

    auto elapsed = timer.GetElapsedInSeconds();
#ifdef SPDEBUG
    std::cout << "Application ran for " << elapsed << " seconds." << std::endl;
#endif
}

void ApplicationOpenGL::Close() {
    glfwSetWindowShouldClose(graphics_->GetWindow(), true);
}

void ApplicationOpenGL::Shutdown() {
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
}   // namespace snowpulse
