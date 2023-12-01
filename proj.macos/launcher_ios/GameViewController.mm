//
//  GameViewController.m
//  launcher_ios
//
//  Created by Marc Jamero on 11/28/23.
//

#import "GameViewController.h"
#import "Renderer.h"

#include <iostream>
#include <snowpulse.h>
#include <game.h>

@implementation GameViewController
{
    MTKView *_view;

    Renderer *_renderer;
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    _view = (MTKView *)self.view;

    _view.device = MTLCreateSystemDefaultDevice();
    _view.backgroundColor = UIColor.greenColor;

    if(!_view.device)
    {
        NSLog(@"Metal is not supported on this device");
        self.view = [[UIView alloc] initWithFrame:self.view.frame];
        return;
    }
    
    auto game = game::Game::Create();
    auto app = static_cast<snowpulse::ApplicationIOS*>(snowpulse::Application::GetInstance());

    if (app->Initialize(snowpulse::Vector2Int(1920, 1080), (__bridge void*)_view)) {
        app->SetGame(game.get());
        game->Initialize(app);
    }
    
    
    //_view.device
    //_renderer = [[Renderer alloc] initWithMetalKitView:_view];

    //[_renderer mtkView:_view drawableSizeWillChange:_view.bounds.size];

    //_view.delegate = _renderer;
}

@end
