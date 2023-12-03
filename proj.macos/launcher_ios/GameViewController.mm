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
    MTKView* view_;

    //Renderer *_renderer;
    std::shared_ptr<game::Game> game_;
    snowpulse::ApplicationIOS* app_;
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    view_ = (MTKView *)self.view;
    view_.device = MTLCreateSystemDefaultDevice();
    view_.backgroundColor = UIColor.greenColor;

    if(!view_.device)
    {
        NSLog(@"Metal is not supported on this device");
        self.view = [[UIView alloc] initWithFrame:self.view.frame];
        return;
    }
    
    game_ = game::Game::Create();
    app_ = static_cast<snowpulse::ApplicationIOS*>(snowpulse::Application::GetInstance());

    if (app_->Initialize(snowpulse::Vector2Int(1242, 2688), (__bridge void*)view_)) {
        app_->SetGame(game_.get());
        game_->Initialize(app_);
    }
    
    
    //_view.device
    //_renderer = [[Renderer alloc] initWithMetalKitView:_view];

    //[_renderer mtkView:_view drawableSizeWillChange:_view.bounds.size];

    //_view.delegate = _renderer;
}


- (void)dealloc {
    app_->Shutdown();
}

@end
