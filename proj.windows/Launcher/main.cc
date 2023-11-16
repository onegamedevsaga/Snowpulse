#include <iostream>
#include <snowpulse.h>
#include <game.h>

int main(int argc, const char* argv[]) {
    auto game = game::Game::Create();
    auto app = snowpulse::ApplicationWindows::Create();

    app->SetGame(game.get());
    game->Initialize(app.get());

    app->Run();
    app->Shutdown();
    return 0;
}
