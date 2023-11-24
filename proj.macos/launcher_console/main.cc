#include <iostream>
#include <snowpulse.h>
#include <game.h>

int main(int argc, const char* argv[]) {
    auto game = game::Game::Create();
    auto app = snowpulse::Application::GetInstance();

    if (app->Initialize()) {
        app->SetGame(game.get());
        game->Initialize(app);

        app->Run();
        app->Shutdown();
    }
    return 0;
}
