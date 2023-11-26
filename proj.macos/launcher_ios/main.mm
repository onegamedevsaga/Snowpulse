//
//  main.m
//  launcher_ios
//
//  Created by Marc Jamero on 11/24/23.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"

#include <iostream>
#include <snowpulse.h>
#include <game.h>

int main(int argc, char * argv[]) {
    NSString * appDelegateClassName;
    @autoreleasepool {
        // Setup code that might create autoreleased objects goes here.
        appDelegateClassName = NSStringFromClass([AppDelegate class]);
    }
    
    auto game = game::Game::Create();
    auto app = snowpulse::Application::GetInstance();

    if (app->Initialize()) {
        app->SetGame(game.get());
        game->Initialize(app);

        app->Run();
        app->Shutdown();
    }
    return UIApplicationMain(argc, argv, nil, appDelegateClassName);
}
