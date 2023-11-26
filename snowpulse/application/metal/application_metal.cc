#include "application_metal.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../timer.h"

namespace snowpulse {
ApplicationMetal::ApplicationMetal() : Application() {
    platform_ = Platform::kMetal;
    platformString_ = "Metal";
}

ApplicationMetal::~ApplicationMetal() {
#ifdef SPDEBUG
    std::cout << "ApplicationMetal destroyed." << std::endl;
#endif
}

bool ApplicationMetal::Initialize(const Vector2Int& resolutionSize, const Vector2Int& screenSize) {
    if (Application::Initialize(resolutionSize, screenSize)) {
#ifdef SPDEBUG
        std::cout << "Starting GLFW context, OpenGLOpenGL 4.1" << std::endl;
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

        window_ = glfwCreateWindow(screenSize.x, screenSize.y, "Snowpulse", NULL, NULL);
        if (!window_) {
#ifdef SPDEBUG
            std::cerr << "Failed to create GLFW window" << std::endl;
#endif
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(window_);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
#ifdef SPDEBUG
            std::cerr << "Failed to initialize GLAD" << std::endl;
#endif
            return false;
        }

        graphics_ = GraphicsMetal::Create();
        if (!graphics_ || !graphics_->Initialize(resolutionSize, screenSize)) {
#ifdef SPDEBUG
            std::cout << "ERROR: Application wasn't able to initialize graphics on this platform." << std::endl;
#endif
            return false;
        }
        
        input_ = static_cast<InputMacOS*>(Input::GetInstance());

#ifdef SPDEBUG
        std::cout << "ApplicationMetal initialized." << std::endl;
#endif
        return true;
    }

    return false;
}

void ApplicationMetal::Run() {
    Timer timer;
    timer.Start();

    Timer frameTimer;
    frameTimer.Start();

    game_->Start();
    actionManager_->Start();

    while (!glfwWindowShouldClose(window_)) {

        glfwPollEvents();
        input_->ProcessInputs(resolutionSize_, screenSize_, window_);

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

        auto batches = graphics_->GetRenderQueue()->PopAllData();
        for (int i = 0; i < 1; i++) {
            for(auto b : batches) {
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

        glfwSwapBuffers(window_);

        graphics_->ClearRenderBatchGroups();
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

void ApplicationMetal::Close() {
    glfwSetWindowShouldClose(window_, true);
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
}   // namespace snowpulse
