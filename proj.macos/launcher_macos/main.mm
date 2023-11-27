#import <Cocoa/Cocoa.h>

#include <iostream>
#include <snowpulse.h>
#include <game.h>

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        // Setup code that might create autoreleased objects goes here.
    }
    
    auto game = game::Game::Create();
    auto app_ = snowpulse::Application::GetInstance();

    if (app_->Initialize(snowpulse::Vector2Int(1920, 1080), snowpulse::Vector2Int(1067, 600))) {
        app_->SetGame(game.get());
        game->Initialize(app_);

        app_->Run();
        app_->Shutdown();
    }
    return 0;//NSApplicationMain(argc, argv);
}
