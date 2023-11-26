//
//  main.m
//  launcher_macos
//
//  Created by Marc Jamero on 11/24/23.
//

#import <Cocoa/Cocoa.h>

#include <iostream>
#include <snowpulse.h>
#include <game.h>

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        // Setup code that might create autoreleased objects goes here.
    }

    auto game = game::Game::Create();
    auto app = snowpulse::Application::GetInstance();

    if (app->Initialize(snowpulse::Vector2Int(1920, 1080), snowpulse::Vector2Int(1067, 600))) {
        app->SetGame(game.get());
        game->Initialize(app);

        app->Run();
        app->Shutdown();
    }

    return 0;
}
