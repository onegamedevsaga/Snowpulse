#include <iostream>
#include <snowpulse.h>
#include <game.h>

int main(int argc, const char* argv[]) {
    auto game = game::Game::Create();
    auto app = static_cast<snowpulse::ApplicationWindows*>(snowpulse::Application::GetInstance());

    auto screenSize = snowpulse::Vector2Int(1244, 700);

    if (app->Initialize(snowpulse::Vector2Int(1920, 1080), screenSize)) {
        app->SetAssetFolderPath("assets/");
        app->SetGame(game.get());
        game->Initialize(app);
        //auto input = static_cast<snowpulse::InputWindows*>(snowpulse::Input::GetInstance());

        auto scaleFactor = 1.0f; //window.screen.backingScaleFactor;
        app->SetScreenScaleFactor(scaleFactor);
    }

    app->Run();
    app->Shutdown();
    return 0;
}